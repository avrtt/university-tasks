#!/usr/bin/env python3

import os, sys, socket, struct, select, time, signal

timeout = 1000
numPackets = 4
packetSize = 64
maxSleep = 1000 

ICMP_ECHO = 8 # echo request type
ICMP_MAX_RECV = 1024 # max size of incoming buffer

class Stats:
    packetsSent = 0
    packetsReceived = 0
    minTime = 10000
    maxTime = 0
    avgTime = 0
    totalTime = 0
    packetLoss = 1.0
    curIP = '0.0.0.0'

stats = Stats
timer = time.time


def ping(hostname):
    
    signal.signal(signal.SIGINT, exitHandler) # Ctrl+C handler
    seqNum = 0 

    try:
        targetIP = socket.gethostbyname(hostname)
        print('ping.py: %s (%s)' % (hostname, targetIP))
    except socket.gaierror as e:
        print('ping.py: Unable to get hostname %s (%s)' % (hostname, e.args[1]))
        sys.exit(0)

    stats.curIP = targetIP

    for i in range(numPackets):
        delay = getDelay(stats, targetIP, hostname, timeout, seqNum, packetSize)
        seqNum += 1

        if delay == None:
            delay = 0

        if (maxSleep > delay):
            time.sleep((maxSleep - delay) / 1000) # ping intervals

    printStats(stats)


def getDelay(stats, targetIP, hostname, timeout, seqNum, packetSize):
    
    delay = None

    try: 
        sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.getprotobyname('ICMP'))
    except socket.error as e:
        print('failed. (Socket error: "%s")' % e.args[1])
        raise

    curProcID = os.getpid() & 0xFFFF

    sentTime = pingSend(sock, targetIP, curProcID, seqNum, packetSize)
    if sentTime == None:
        sock.close()
        return delay

    stats.packetsSent += 1

    recvTime, dataSize, iphSrcIP, icmpSeqNumber, iphTTL = pingRcv(sock, curProcID, timeout)
    sock.close()

    if recvTime:
        delay = (recvTime - sentTime) * 1000
        print('%d bytes from %s: icmp_seq=%d ttl=%d time=%d ms' % (dataSize, socket.inet_ntoa(struct.pack('!I', iphSrcIP)), icmpSeqNumber, iphTTL, delay))
        stats.packetsReceived += 1
        stats.totalTime += delay
        if stats.minTime > delay:
            stats.minTime = delay
        if stats.maxTime < delay:
            stats.maxTime = delay
    else:
        delay = None
        print('Request timed out.')

    return delay


def pingSend(sock, targetIP, ID, seqNum, packetSize):

    padBytes = []
    startVal = 0x42 # 66
    checksum = 0

    # ICMP header                 type, code (8)   (16)   (16)  (16)      
    header = struct.pack('!BBHHH', ICMP_ECHO, 0, checksum, ID, seqNum)

    # (packetSize - 8) removes header size from packet size
    for i in range(startVal, startVal + (packetSize - 8)):
        padBytes += [(i & 0xff)] # keep chars in the 0-255 range
    
    data = bytearray(padBytes) # convert list to byte array
    checksum = getChecksum(header + data)
    header = struct.pack('!BBHHH', ICMP_ECHO, 0, checksum, ID, seqNum)
    packet = header + data # packet to send

    sendTime = timer()

    try:
        sock.sendto(packet, (targetIP, 1)) # the port number is irrelevant
    except socket.error as e:
        print('General failure (%s)' % (e.args[1]))

    return sendTime


def pingRcv(sock, ID, timeout):

    timeLeft = timeout / 1000

    while True: # loop while waiting for packet or timeout
        startMonitoring = timer()
        whatReady = select.select([sock], [], [], timeLeft) # monitoring socket
        monitoringTime = (timer() - startMonitoring)
        if whatReady[0] == []: # timeout
            return None, 0, 0, 0, 0

        timeReceived = timer()

        recPacket, addr = sock.recvfrom(ICMP_MAX_RECV)

        ipHeader = recPacket[:20]
        iphVersion, iphTypeOfSvc, iphLength, iphID, iphFlags, iphTTL, iphProtocol, iphChecksum, iphSrcIP, iphDestIP = struct.unpack('!BBHHHBBHII', ipHeader)

        icmpHeader = recPacket[20:28]
        icmpType, icmpCode, icmpChecksum, icmpPacketID, icmpSeqNumber = struct.unpack('!BBHHH', icmpHeader)

        if icmpPacketID == ID: # if our packet
            dataSize = len(recPacket) - 28
            return timeReceived, (dataSize + 8), iphSrcIP, icmpSeqNumber, iphTTL

        timeLeft = timeLeft - monitoringTime
        if timeLeft <= 0:
            return None, 0, 0, 0, 0


def getChecksum(sourceString):

    sum, numPackets, loByte, hiByte = 0, 0, 0, 0
    numPacketsTo = (int(len(sourceString) / 2)) * 2

    # handle bytes in pairs (decoding as short ints)
    while numPackets < numPacketsTo:
        loByte, hiByte = sourceString[numPackets], sourceString[numPackets + 1]
        sum = sum + (hiByte * 256 + loByte)
        numPackets += 2

    # handle last byte (if the number of bytes is odd)
    if numPacketsTo < len(sourceString): # check for odd length
        loByte = sourceString[len(sourceString)-1]
        sum += loByte

    sum = (sum >> 16) + (sum & 0xffff) # add high 16 bits to low 16 bits
    sum += (sum >> 16) # add carry from above (if any)
    answer = ~sum & 0xffff # invert and truncate to 16 bits
    answer = socket.htons(answer)

    return answer


def printStats(stats):

    print('\n--- %s ping statistics ---' % (stats.curIP))

    if stats.packetsSent > 0:
        stats.packetLoss = 100.0 * (stats.packetsSent - stats.packetsReceived) / stats.packetsSent

    print('%d packets transmitted, %d packets received, %0.1f%% packet loss' % (stats.packetsSent, stats.packetsReceived, stats.packetLoss))

    if stats.packetsReceived > 0:
        print('min/avg/max = %d/%0.1f/%d ms' % (stats.minTime, stats.totalTime / stats.packetsReceived, stats.maxTime))


def exitHandler(frame, signal):

    printStats(stats)
    print('\n(Terminated)')

    sys.exit(0)


if __name__ == '__main__':
    ping(sys.argv[1])


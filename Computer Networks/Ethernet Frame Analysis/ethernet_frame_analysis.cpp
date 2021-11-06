#include <stdio.h>
#include <conio.h> 
#include <string.h>

const int maxFrameLength = 0x05FE;
const int Ethernet802_3 = 0xFFFF;
const int EthernetSNAP = 0xAA;
const int numMAC = 6;
int numRaw = 0;
int numSNAP = 0;
int numLLC = 0;
int numIPv4 = 0;
int numARP = 0;
int readFrame(FILE* in);
int readDatagram(FILE* in);
int BPDU(FILE* in);
int ARP(FILE* in);
void printStat(int);

int main()
{
	char fileName[20];
	int i = 1;

	printf("Enter the file name: ");
	gets_s(fileName);
	FILE* in;
	fopen_s(&in, fileName, "rb+");

	do
	{
		printf("\nFrame %d\n", i);
		i++;
	} while (!readFrame(in));

	printStat(i);

	return 0;
}

int readFrame(FILE* in)
{
	int i;
	bool flag = false;
	unsigned char MAC[numMAC], BUF[2], OUI[3], LLC3;
	unsigned short int orgType, firstTwoBytes, typeLength;

	while (!flag)
	{
		if (fread(MAC, 1, numMAC, in) != numMAC)
			return 1;
		for (i = 0; i < numMAC; i++)
			flag = flag || MAC[i];
	}

	printf("MAC (Target): ");
	printf("%02X", MAC[0]);
	for (i = 1; i < numMAC; i++)
		printf("-%02X", MAC[i]);

	fread(MAC, 1, numMAC, in);
	printf("\nMAC (Sender): ");
	printf("%02X", MAC[0]);
	for (i = 1; i < numMAC; i++)
		printf("-%02X", MAC[i]);

	fread(BUF, 1, 2, in);
	typeLength = (BUF[0] << 8) | BUF[1];

	if (typeLength > maxFrameLength)
	{
		printf("\nFrame Type: Ethernet II / IPv4 (%04X)\n", typeLength);
		if (typeLength == 0x0800)
		{
			readDatagram(in);
			numIPv4++;
		}
		if (typeLength == 0x0806)
		{
			ARP(in);
			numARP++;
		}
	}
	else
	{
		fread(BUF, 1, 2, in);
		firstTwoBytes = (BUF[0] << 8) | BUF[1];
		if (firstTwoBytes == Ethernet802_3)
		{
			printf("\nFrame Type: Ethernet 802.3 Raw\n");
			numRaw++;
			printf("   Length: %d", firstTwoBytes);
			readDatagram(in);
		}
		else if (BUF[0] == EthernetSNAP && BUF[1] == EthernetSNAP)
		{
			printf("\nFrame Type: EthernetSNAP\n");

			fread(&LLC3, 1, 1, in);
			printf("   LLC: %2X-%2X-%2X\n", BUF[0], BUF[1], LLC3);
			fseek(in, 1, SEEK_CUR);

			fread(OUI, 1, 3, in);
			printf("   Organization by Standart:");
			for (i = 0; i < 2; i++)
				printf("%02X-", OUI[i]);
			printf("%02X\n", OUI[2]);

			fread(BUF, 1, 2, in);
			orgType = (BUF[0] << 8) | BUF[1];
			printf("   Frame Type by Organization: (%04X)\n", orgType);

			readDatagram(in);
			numSNAP++;
		}
		else
		{
			printf("\nFrame Type: Ethernet 802.3/LLC\n");

			fread(&LLC3, 1, 1, in);
			printf("   LLC: %0X-%0X-%0X\n", BUF[0], BUF[1], LLC3);

			if (BUF[0] == 0x6 && BUF[1] == 0x6)
				readDatagram(in);
			if (BUF[0] == 0x42 && BUF[1] == 0x42)
				BPDU(in);
			numLLC++;
		}
	}
	return 0;
}

int readDatagram(FILE* in)
{
	int i;
	unsigned short int datagramLength, ident, BUF2, checksum;
	unsigned char BUF, TOS, TTL, protocol, bufArr[2], ipSender[4], ipTarget[4];
	fread(&BUF, 1, 1, in);
	unsigned char version = (BUF & 0xF0) >> 4;
	unsigned char headerLength = BUF & 0x0F;
	unsigned char flags;
	unsigned short offset;

	printf("   Version: %X\n", version);
	printf("   Header Lenght: %d bytes\n", headerLength);

	fread(&TOS, 1, 1, in);
	printf("   TOS: %X\n", TOS);

	fread(&bufArr, 2, 1, in);
	datagramLength = (bufArr[0] << 8) | bufArr[1];
	printf("   Datagram Lenght: %d bytes\n", datagramLength);

	fread(&bufArr, 2, 1, in);
	ident = (bufArr[0] << 8) | bufArr[1];
	printf("   Identificator: %X\n", ident);

	fread(&bufArr, 2, 1, in);
	BUF2 = (bufArr[0] << 8) | bufArr[1];

	flags = (BUF2 & 0xE000) >> 13;
	printf("   Flags: %X\n", flags);

	offset = (BUF2 & 0x1FFF);
	printf("   Fragment Offset: %d\n", offset);

	fread(&TTL, 1, 1, in);
	printf("   TTL: %d\n", TTL);

	fread(&protocol, 1, 1, in);
	printf("   Protocol of Higher Level: %d\n", protocol);

	fread(&bufArr, 2, 1, in);
	checksum = (bufArr[0] << 8) | bufArr[1];
	printf("   Header Checksum: %d\n", checksum);

	fread(&ipSender, 1, 4, in);
	printf("   IP (Sender): ");
	for (i = 0; i < 3; i++)
		printf("%d.", ipSender[i]);
	printf("%d\n", ipSender[3]);

	printf("   IP (Target): ");
	fread(&ipTarget, 1, 4, in);
	for (i = 0; i < 3; i++)
		printf("%d.", ipTarget[i]);
	printf("%d\n", ipTarget[3]);

	fseek(in, datagramLength - 20, SEEK_CUR);

	return 0;
}

int BPDU(FILE* in)
{
	int i;
	unsigned int rootPathCost;
	unsigned char BUF, BUF2[2], BUF4[4], BUF8[8];
	short unsigned protID, portIdent, msgAge, maxAge, helloTime, forwardDelay;

	fread(&protID, 2, 1, in);
	printf("   Protocol Identifier: %X\n", protID);

	fread(&BUF, 1, 1, in);
	printf("   Protocol Version Identifier: %X\n", BUF);

	fread(&BUF, 1, 1, in);
	if (BUF == 0x00)
		printf("   BPDU Type: Configurational\n");
	else
		printf("   BPDU Type: Topology Changed\n");

	fread(&BUF, 1, 1, in);
	printf("   Rags: %X\n", BUF);

	fread(&BUF8, 1, 8, in);
	printf("   Root Identifier: ");
	for (i = 0; i < 7; i++)
		printf("%X-", BUF8[i]);
	printf("%X\n", BUF8[7]);

	fread(&BUF4, 1, 4, in);
	rootPathCost = (BUF4[0] << 32) | (BUF4[1] << 16) | (BUF4[2] << 8) | BUF4[3];
	printf("   Root Path Cost: %d\n", rootPathCost);

	fread(&BUF8, 1, 8, in);
	printf("   Bridge Identifier: ");
	for (i = 0; i < 7; i++)
		printf("%X-", BUF8[i]);
	printf("%X\n", BUF8[7]);

	fread(&BUF2, 1, 2, in);
	portIdent = (BUF2[0] << 8) | BUF2[1];
	printf("   Port Identifier: %d\n", portIdent);

	fread(&BUF2, 1, 2, in);
	msgAge = (BUF2[0] << 8) | BUF2[1];
	printf("   Message Age: %d\n", msgAge);

	fread(&BUF2, 1, 2, in);
	maxAge = (BUF2[0] << 8) | BUF2[1];
	printf("   Max Age: %d\n", maxAge);

	fread(&BUF2, 1, 2, in);
	helloTime = (BUF2[0] << 8) | BUF2[1];
	printf("   Hello Time: %d\n", helloTime);

	fread(&BUF2, 1, 2, in);
	forwardDelay = (BUF2[0] << 8) | BUF2[1];
	printf("   Forward Delay: %d\n", forwardDelay);

	return 0;
}

int ARP(FILE* in)
{
	int i;
	unsigned char hlen, plen, BUF2[2], BUF4[4];
	unsigned char *bufnh, *bufnp;
	short unsigned hwType, protocolType, operation;

	fread(&BUF2, 1, 2, in);
	hwType = (BUF2[0] << 8) | BUF2[1];
	printf("   Hardware Type: %X\n", hwType);

	fread(&BUF2, 1, 2, in);
	protocolType = (BUF2[0] << 8) | BUF2[1];
	printf("   Protocol Type: %X\n", protocolType);

	fread(&hlen, 1, 1, in);
	printf("   Hardware Length: %d\n", hlen);

	fread(&plen, 1, 1, in);
	printf("   Protocol length: %d\n", plen);

	bufnh = new unsigned char[hlen];
	bufnp = new unsigned char[plen];

	fread(&BUF2, 1, 2, in);
	operation = (BUF2[0] << 8) | BUF2[1];
	printf("   Operation: %X\n", operation);

	fread(bufnh, 1, hlen, in);
	printf("   Sender Hardware Address: ");
	for (i = 0; i < hlen - 1; i++)
		printf("%02X-", bufnh[i]);
	printf("%02X\n", bufnh[i]);

	fread(bufnp, 1, plen, in);
	printf("   Sender Protocol Address: ");
	for (i = 0; i < plen - 1; i++)
		printf("%d.", bufnp[i]);
	printf("%d\n", bufnp[i]);

	fread(bufnh, 1, hlen, in);
	printf("   Target Hardware Address: ");
	for (i = 0; i < hlen - 1; i++)
		printf("%02X-", bufnh[i]);
	printf("%02X\n", bufnh[i]);

	fread(bufnp, 1, plen, in);
	printf("   Target Protocol Address: ");
	for (i = 0; i < plen - 1; i++)
		printf("%d.", bufnp[i]);
	printf("%d\n", bufnp[i]);

	return 0;
}

void printStat(int i)
{
	printf("\n");
	printf("Number of Frames: %d\n", i - 2);
	printf("Number of IPv4 Frames: %d\n", numIPv4);
	printf("Number of LLC Frames: %d\n", numLLC);
	printf("Number of ARP Frames: %d\n", numARP);
	printf("Number of Raw Frames: %d\n", numRaw);
	printf("Number of SNAP Frames: %d\n", numSNAP);
}

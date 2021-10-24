#pragma comment (lib,"Ws2_32.lib")
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string>
using namespace std;
bool offline; // client status flag
const int U = 256; // username length
const int M = 1000; // message length

DWORD WINAPI receive(LPVOID clientSocket)
{
    int retVal; // return value for error check
    char Resp[M]; // response

    SOCKET clientSock;
    clientSock = *((SOCKET*)clientSocket);

    // message from the server
    retVal = recv(clientSock, Resp, M, 0);

    // if the server is down
    if (!strcmp(Resp, "Server shutdown."))
    {
        cout << "Server shutdown." << endl;
        offline = true;
        return 0;
    }

    // if the server is full
    if (!strcmp(Resp, "Server is full, please try again later."))
    {
        cout << "Server is full, please try again later." << endl;
        offline = true;
        return 0;
    }

    // if the client is still working
    if (!offline)
    {
        if (retVal == SOCKET_ERROR)
        {
            retVal = 0;
            cout << "Error: Unable to receive message!" << endl;
            offline = true;
            return 0;
        }
        else
        {
            // print server message
            cout << Resp << endl;
        }
    }
    return 1;
}

DWORD WINAPI send(LPVOID clientSocket)
{
    int retVal;
    char Buf[M]; // buffer array
    SOCKET clientSock;
    clientSock = *((SOCKET*)clientSocket);
    gets_s(Buf);

    // if the user entered /q (quit)
    if (!strcmp(Buf, "/q"))
    {
        offline = true;
        retVal = send(clientSock, Buf, M, 0);
        return 0;
    }
    else
    {
        // send message to the server
        retVal = send(clientSock, Buf, M, 0);

        if (retVal == SOCKET_ERROR)
        {
            cout << "Error: Unable to send message!" << endl;
            WSACleanup();
            system("pause");
            return 0;
        }
    }
    return 1;
}

int main()
{
    string ip;
    WSADATA wsaData;
    int retVal = 0;
    offline = false;
    char username[U];
    WORD ver = MAKEWORD(2, 2);

    WSAStartup(ver, (LPWSADATA)&wsaData);
    SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSock == SOCKET_ERROR)
    {
        cout << "Error: Unable to create socket!" << endl;
        WSACleanup();
        system("pause");
        return 1;
    }

    // reading IP
    cout << "IP > ";
    cin >> ip;
    cin.ignore();

    SOCKADDR_IN serverInfo;
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(2008);
    inet_pton(AF_INET, ip.c_str(), &serverInfo.sin_addr);

    // connecting to the server
    retVal = connect(clientSock, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));

    if (retVal == SOCKET_ERROR)
    {
        cout << "Error: Unable to connect to the server!" << endl;
        WSACleanup();
        system("pause");
        return 1;
    }

    // reading username
    cout << "Connected!" << endl;
    cout << "Username: ";
    cin >> username;

    // send username to the server
    retVal = send(clientSock, username, U, 0);

    if (retVal == SOCKET_ERROR)
    {
        cout << "Error: Unable to send username!" << endl;
        WSACleanup();
        return 1;
    }

    cout << "Welcome to the chat room. Use /q to exit." << endl;

    // while the client/server is running
    while (!offline)
    {
        DWORD threadID;
        CreateThread(NULL, NULL, send, &clientSock, NULL, &threadID);
        CreateThread(NULL, NULL, receive, &clientSock, NULL, &threadID);
    }

    closesocket(clientSock);
    WSACleanup();
    return 0;
}

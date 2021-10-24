#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib,"Ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
const int U = 256; // username lenght
const int M = 1000; // message lenght
const int maxClients = 5; // maximum number of users
int curClients; // number of connected users
char usernames[maxClients + 1][U];

SOCKET servSock;
SOCKET clSockets[maxClients];
SOCKADDR_IN clSADDR[maxClients];
USHORT ports[maxClients];

DWORD WINAPI chat(LPVOID clientSocket)
{
    int retVal; // return value for error check
    char Req[M]; // request
    char Resp[M]; // response
    int i, j, cur;

    SOCKET clientSock;
    clientSock = *((SOCKET*)clientSocket);

    while (true)
    {
        // message from the client
        retVal = recv(clientSock, Req, M, 0);

        if (retVal == SOCKET_ERROR)
        {
            cout << "Error: Unable to receive message!" << endl;
            closesocket(clientSock);
            cout << "Connection closed." << endl;
            return SOCKET_ERROR;
        }
        else
        {
            // if message is empty
            if (retVal >= M) retVal = M - 1;
            Req[retVal] = '\0';
        }

        cout << "Data received." << endl;

        SOCKADDR_IN sin;
        for (i = 0; i < curClients; i++)
        {
            if (clSockets[i] == clientSock)
            {
                sin = clSADDR[i];
            }
        }

        cur = 0;
        while (ports[cur] != sin.sin_port) // username search
        {
            cur++;
        }

        if (!strcmp(Req, "/q")) // if the user entered /q (quit)
        {
            // notify all clients that the user is logged out
            Resp[0] = '\0';
            strcat_s(Resp, usernames[cur]);
            strcat_s(Resp, " left the chat.");

            for (i = 0; i < curClients; i++)
            {
                if (clSockets[i] != clientSock) retVal = send(clSockets[i], Resp, M, 0);
            }

            if (retVal == SOCKET_ERROR)
            {
                cout << "Error: Unable to send message!" << endl;
                return SOCKET_ERROR;
            }

            cout << "Client disconnected." << endl;
            closesocket(clientSock);
            cout << "Connection closed." << endl;

            // delete user information
            for (j = cur; j < curClients; j++)
            {
                clSockets[j] = clSockets[j + 1];
                clSADDR[j] = clSADDR[j + 1];
                ports[j] = ports[j + 1];
                strcpy_s(usernames[j], usernames[j + 1]);
            }

            clSockets[curClients - 1] = SOCKET_ERROR;
            curClients--;
            cout << "Current online: " << curClients << endl; // print the updated number of online users
            return SOCKET_ERROR;
        }

        if (Req[0] != '\0') // if the received message isn't an empty string
        {
            // print username and message
            cout << usernames[cur] << ": " << Req << endl;
            Resp[0] = '\0';
            strcat_s(Resp, usernames[cur]);
            strcat_s(Resp, ": ");
            strcat_s(Resp, Req);

            cout << "Sending response from the server." << endl;

            // send message to other users
            for (i = 0; i < curClients; i++)
            {
                if (clSockets[i] != clientSock) retVal = send(clSockets[i], Resp, M, 0);
            }

            if (retVal == SOCKET_ERROR)
            {
                cout << "Error: Unable to send message!" << endl;
                return SOCKET_ERROR;
            }
        }
    }
}

int main()
{
    int retVal;
    int i;
    char NewClient[M];

    curClients = 0;

    WORD sockVer;
    WSADATA wsaData;
    sockVer = MAKEWORD(2, 2);
    WSAStartup(sockVer, &wsaData);

    // creating socket
    servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (servSock == INVALID_SOCKET)
    {
        cout << "Error: Unable to create socket!" << endl;
        WSACleanup();
        system("pause");
        return SOCKET_ERROR;
    }

    for (i = 0; i < maxClients; i++)
    {
        clSockets[i] = SOCKET_ERROR;
    }

    SOCKADDR_IN sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(2008);
    sin.sin_addr.s_addr = INADDR_ANY;

    // binding socket
    retVal = bind(servSock, (LPSOCKADDR)&sin, sizeof(sin));

    if (retVal == SOCKET_ERROR)
    {
        cout << "Error: Unable to bind socket!" << endl;
        WSACleanup();
        system("pause");
        return SOCKET_ERROR;
    }

    cout << "Server started at port: " << htons(sin.sin_port) << endl;

    while(true)
    {
        // starting to listen
        retVal = listen(servSock, 10);

        if (retVal == SOCKET_ERROR)
        {
            cout << "Error: Unable to listen socket!" << endl;
            WSACleanup();
            system("pause");
            return SOCKET_ERROR;
        }

        SOCKET clientSock;
        SOCKADDR_IN from;
        int fromlen = sizeof(from);

        // accepting the client's request
        clientSock = accept(servSock, (struct sockaddr*)&from, &fromlen);

        if (clientSock == INVALID_SOCKET)
        {
            cout << "Error: Unable to accept socket!" << endl;
            WSACleanup();
            system("pause");
            return SOCKET_ERROR;
        }

        cout << "New connection: " << inet_ntoa(from.sin_addr) << ":" << htons(from.sin_port) << endl;
        cout << "Users: " << curClients + 1 << endl;

        // get username of a new client
        retVal = recv(clientSock, usernames[curClients], U, 0);

        // get client data
        if (retVal == SOCKET_ERROR)
        {
            cout << "Error: Unable to recieve message!" << endl;
            system("pause");
            return SOCKET_ERROR;
        }

        // if username is /shutdown
        if (!strcmp(usernames[curClients], "/shutdown"))
        {
            // send "Server shutdown" to all clients and close sockets
            for (i = 0; i < curClients; i++)
            {
                retVal = send(clSockets[i], "Server shutdown.", M, 0);
                closesocket(clSockets[i]);
            }

            retVal = send(clientSock, "Server shutdown.", M, 0);
            closesocket(clientSock);
            break;
        }
        else
        {
            // if the maximum number of users isn't reached
            if (curClients < maxClients)
            {
                // save and print information about the current user
                ports[curClients] = from.sin_port;
                clSockets[curClients] = clientSock;
                clSADDR[curClients] = from;
                NewClient[0] = '\0';

                strcat_s(NewClient, "Client ");
                strcat_s(NewClient, usernames[curClients]);
                strcat_s(NewClient, " connected (IP: ");
                strcat_s(NewClient, inet_ntoa(from.sin_addr));
                strcat_s(NewClient, ")");
                cout << NewClient << endl;

                curClients++;

                // send a message to other clients about the new user
                for (i = 0; i < curClients; i++)
                {
                    if (clSockets[i] != clientSock) retVal =
                        send(clSockets[i], NewClient, M, 0);
                }
            }
            else
            {
                // if the maximum number of users is reached
                cout << "Max online." << endl;
                retVal = send(clientSock, "Server is full, please try again later.", U, 0);
                closesocket(clientSock);
                cout << "Connection closed." << endl;
            }

            DWORD threadID;
            CreateThread(NULL, NULL, chat, &clientSock, NULL, &threadID);
        }
    }

    closesocket(servSock);
    WSACleanup();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#define BUF_SIZE 100
#define PORT 2008
 
int main() {
 
    int s_server, s_new; // sockets
    int number1, number2, sum; // numbers to add and their sum
    struct sockaddr_in serv_addr;
    struct sockaddr_in from;
    char buffer_in[BUF_SIZE] = {0};
    char buffer_out[BUF_SIZE] = {0};
    int from_len = sizeof(from);
 
    // creating socket
    if ((s_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) != 0) {
        printf("Socket opened\n"); 
    }
    else {
        printf("Error: Socket was not opened!\n");
        exit(EXIT_FAILURE);
    }
 
    // socket parameters
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
 
    // bind server socket to specific port
    if (bind(s_server, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == SO_ERROR) {
        printf("Error: Socket was not bound!\n");
        printf("Socket closed\n");
        exit(EXIT_FAILURE);
    }
 
    // starting socket listening
    if (listen(s_server, 5) < 0) { // max queue size = 5
        printf("Error!\n");
        printf("Socket closed\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Started listening the port %d\n", PORT);
    }
 
    // accepting a client connection
    s_new = accept(s_server, (struct sockaddr *) &from, (socklen_t *) & from_len);
    if (s_new < 0) {
        printf("Error: Connection was not accepted!\n");
        printf("Socket closed\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Connection accepted from %s (port: %d)\n", inet_ntoa(from.sin_addr), htons(from.sin_port));
    }
 
    // receiving messages from the client
    recv(s_new, buffer_in, BUF_SIZE);
    number1 = atoi(buffer_in);
    printf("Message received: %d\n", number1);
 
    recv(s_new, buffer_in, BUF_SIZE);
    number2 = atoi(buffer_in);
    printf("Message received: %d\n", number2);
 
    sum = number1 + number2;
 
    // sending to the client
    snprintf(buffer_out, BUF_SIZE, "%d", sum);
    send(s_new, buffer_out, BUF_SIZE, 0);
    printf("Answer sent\n");
 
    // closing server socket
    close(s_new);
    printf("Connection closed\n");
 
    close(s_server);
    printf("Socket closed\n");
 
    return 0;
}

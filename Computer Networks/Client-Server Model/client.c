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
#define SERVER_IP "192.168.1.102"
 
int main() {
 
    int s_client; // socket
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE] = {0};
 
    // creating socket
    if ((s_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error: Socket was not opened!");
        printf("Socket closed\n");
        exit(EXIT_FAILURE);
    }
 
    printf("Socket opened\n");
 
    // set server port
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_family = AF_INET;
 
    // set IP
    if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Error: Invalid IP-adress!");
        exit(EXIT_FAILURE);
    }
 
    // connecting to the server
    if (connect(s_client, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error: Connection was not established!");
        printf("Socket closed\n");
        exit(EXIT_SUCCESS);
    }
 
    printf("Connected to the server\n");
 
    // sending messages to the server
    printf("Enter the number 1: ");
    fgets(buffer, BUF_SIZE, stdin);
    send(s_client, buffer, BUF_SIZE, 0);
    printf("Message sent\n");
 
    printf("Enter the number 2: ");
    fgets(buffer, BUF_SIZE, stdin);
    send(s_client, buffer, BUF_SIZE, 0);
    printf("Message sent\n");
 
 
    // receiving messages from the server
    read(s_client, buffer, BUF_SIZE);
    printf("Answer received: %s\n", buffer);
 
    // closing socket
    close(s_client);
 
    printf("Socket closed\n");
   
    return 0;
}

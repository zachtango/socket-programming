#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/Practical.h"

/*
    4 General Steps for TCP Client Communication:
    1. Create TCP socket using socket()
    2. Establish connection to server using connect()
    3. Communicate using send() and recv()
    4. Close connection with close()

*/

int main(int argc, char *argv[]) {
    
    /* PARAMETER PARSING AND CHECKING */
    if (argc < 3 || argc > 4) {
        DieWithUserMessage("Parameters",
            "<Server Address> <Echo Word> [<Server Port>]");
    }

    char *servIP = argv[1]; // 1st arg: IP Address of server
    char *echoString = argv[2]; // 2nd arg: string to echo
    
    // 3rd arg (optional): server port (int), defaults to 7 a well-known echo port
    in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 7;

    /* TCP SOCKET CREATION */
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        DieWithSystemMessage("socket() failed");
    }

    /* PREPARE ADDRESS AND ESTABLISH CONNECTION */
    // Prepare sockaddr_in structure to hold server addresses
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    
    // Fill in sockaddr_in
    int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr); // IP char* to int representation
    if (rtnVal == 0) {
        DieWithUserMessage("inet_pton() failed", "invalid address string");
    } else if (rtnVal < 0) {
        DieWithSystemMessage("inet_pton() failed");
    }
    servAddr.sin_port = htons(servPort);

    // Connect
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        DieWithSystemMessage("connect() failed");
    }

    /* SEND ECHO STRING */
    size_t echoStringLen = strlen(echoString);

    ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
    if (numBytes < 0) {
        DieWithSystemMessage("send() failed");
    } else if ((size_t) numBytes != echoStringLen) {
        DieWithUserMessage("send()", "sent unexpected number of bytes");
    }

    /* RECEIVE ECHO REPLY */
    unsigned int totalBytesRcvd = 0;
    while (totalBytesRcvd < echoStringLen) {
        char buffer[BUFSIZ];
        /* Receive up to buffer size (minus 1 to leave space for
            null terminator) bytes from sender
        */
        numBytes = recv(sock, buffer, BUFSIZ - 1, 0);
        if (numBytes < 0) {
            DieWithSystemMessage("recv() failed");
        } else if (numBytes == 0) {
            DieWithUserMessage("recv()", "connection closed prematurely");
        }

        totalBytesRcvd += numBytes;
        buffer[numBytes] = '\0';
        fputs(buffer, stdout);
    }

    fputc('\n', stdout);

    close(sock);
    return 0;
}


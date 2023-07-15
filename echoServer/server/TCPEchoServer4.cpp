#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/Practical.h"
#include "../include/TCPServerUtility.h"

/*
    4 General Steps for basic TCP Server Communication:
    1. Create TCP socket using socket()
    2. Assign port number to socket with bind()
    3. Tell system to allow connections to be made to that port using listen()
    4. Repeatedly do following:
        - Call accept() to get new socket for each client connection
        - Communicate with client via new socket using send() and recv()
        - Close client connection using close()
*/

// Max outstanding connection requests
static const int MAXPENDING = 5;

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        DieWithUserMessage("Parameters", "<Server Port>");
    }

    in_port_t servPort = atoi(argv[1]); // 1st arg: local port

    /* SOCKET CREATION AND SETUP */
    int servSock;
    if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        DieWithSystemMessage("socket() failed");
    }

    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Let system pick addr
    servAddr.sin_port = htons(servPort);

    if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        DieWithSystemMessage("bind() failed");
    }

    // Retrieve the local IP address of the server
    struct sockaddr_in localAddr;
    socklen_t addrLen = sizeof(localAddr);
    if (getsockname(servSock, (struct sockaddr *)&localAddr, &addrLen) < 0) {
        DieWithSystemMessage("getsockname() failed");
    }

    char localIP[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &localAddr.sin_addr, localIP, sizeof(localIP)) != NULL) {
        printf("Server IP: %s\n", localIP);
    } else {
        puts("Unable to get server IP address");
    }


    if (listen(servSock, MAXPENDING) < 0) {
        DieWithSystemMessage("listen() failed");
    }

    /* ITERATIVELY HANDLE INCOMING CONNECTIONS */
    for (;;) {
        struct sockaddr_in clntAddr;
        socklen_t clntAddrLen = sizeof(clntAddr);

        int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
        if (clntSock < 0) {
            DieWithSystemMessage("accept() failed");
        }

        // clntSock connected to a client!

        char clntName[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr,
                        clntName, sizeof(clntName)) != NULL) {
            printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
        } else {
            puts("Unable to get client address");
        }
        HandleTCPClient(clntSock);
    }

    return 0;
}

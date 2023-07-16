#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/Practical.h"
#include <pthread.h>
#include <unistd.h>

#include <unordered_map>
using std::unordered_map;
#include <string>
using std::string;
using std::to_string;

/*
    4 Steps:
    1. create a socket
    2. bind the socket to an IP address and port
    3. listen on the socket
    4. accept on the socket (block until you get a request to connect from client)
*/

typedef struct ThreadArgs {
    int clntSock;
    int userId;

    ThreadArgs(int clntSock, int userId) : clntSock(clntSock), userId(userId) {}
} ThreadArgs;

const int MAXPENDING = 5;
int lastUserId = 0;
unordered_map<int, int> userIdToClntSock;
string userIdList;

void generateUserIdList() {
    string newUserIdList;
    for(auto &e : userIdToClntSock) {
        newUserIdList += to_string(e.first) + ' ';
    }
    userIdList = newUserIdList;
}

int getUserIdFromMessage(char *msg) {
    if( !isdigit(msg[0]) ) {
        return -1;
    }

    char buffer[BUFSIZ];

    int i = 0;
    while( isdigit(msg[i]) ) {
        buffer[i] = msg[i];
        i += 1;
    }

    buffer[i] = '\0';

    return atoi(buffer);
}

void SendToUser(int userId, const char *buffer, size_t size) {
    ssize_t numBytesSent = send(userIdToClntSock[userId], buffer, size, 0);
    if (numBytesSent < 0) {
        DieWithSystemMessage("send() failed");
    } else if ( (size_t) numBytesSent != size ) {
        DieWithUserMessage("send()", "sent unexpected number of bytes");
    }

}

void *HandleTCPClient(void *arg) {
    ThreadArgs *args = (ThreadArgs *) arg;
    int clntSock = args->clntSock;
    int userId = args->userId;

    char buffer[BUFSIZ];

    string msg = "User Ids: " + userIdList + '\n' +
        "<User Id> <msg to send to user>\n";
    SendToUser(userId, msg.c_str(), msg.length());
    
    ssize_t numBytesRcvd = recv(clntSock, buffer, BUFSIZ, 0);
    if (numBytesRcvd < 0) {
        DieWithSystemMessage("recv() failed");
    }

    while (numBytesRcvd > 0) {
        int partnerUserId = getUserIdFromMessage(buffer);
        int partnerUserIdLength = to_string(partnerUserId).length();

        if( partnerUserId == -1 || userIdToClntSock.count(partnerUserId) == 0 ) {
            msg = "Invalid user id, please select an id from the provided: " +
                    userIdList + '\n' +
                    "User Ids: " + userIdList + '\n' +
                    "<User Id> <msg to send to user>\n";
            SendToUser(userId, msg.c_str(), msg.length());
        } else {
            // send msg to partner
            SendToUser(partnerUserId, &buffer[partnerUserIdLength + 1], numBytesRcvd - partnerUserIdLength - 1);
        }

        numBytesRcvd = recv(clntSock, buffer, BUFSIZ, 0);
        if (numBytesRcvd < 0) {
            printf("recv() failed");
        }
    }

    printf("Closing user %d connection\n", userId);

    userIdToClntSock.erase(userId);
    close(clntSock);

    generateUserIdList();

    return NULL;
}

int main(int argc, char *argv[]) {

    // Arg checking
    if(argc != 2) {
        DieWithUserMessage("Usage", "./a.out <Port>");
    }
    int servPort = atoi(argv[1]);

    // Create socket
    int servSock;
    if( (servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
        DieWithSystemMessage("socket() failed");
    }

    // Bind socket
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(servPort);
    if( bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0 ) {
        DieWithSystemMessage("bind() failed");
    }

    // Listen on socket
    if( listen(servSock, MAXPENDING) < 0 ) {
        DieWithSystemMessage("listen() failed");
    }

    // Run server forever
    for(;;) {
        struct sockaddr_in clntAddr;
        socklen_t clntAddrLen = sizeof(clntAddr);

        int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
        if (clntSock < 0) {
            DieWithSystemMessage("accept() failed");
        }

        char clntName[INET_ADDRSTRLEN];
        if ( inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr,
                        clntName, sizeof(clntName)) != NULL ) {
            printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
        } else {
            printf("Unable to get client address\n");
        }
        
        userIdToClntSock[lastUserId] = clntSock;
        generateUserIdList();

        pthread_t thread;
        ThreadArgs args {clntSock, lastUserId};
        int result = pthread_create(&thread, NULL, HandleTCPClient, (void *) &args);
        if (result == 0) {
            printf("Handling user %d in thread\n", lastUserId);
        } else {
            printf("Unable to create thread for user %d\n", lastUserId);
            userIdToClntSock.erase(lastUserId);
            generateUserIdList();
        }
        
        lastUserId += 1;
    }

    return 0;
}

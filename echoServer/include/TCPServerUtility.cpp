#include "../include/TCPServerUtility.h"
#include "../include/Practical.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void HandleTCPClient(int clntSocket) {
    char buffer[BUFSIZ];

    ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZ, 0);
    if (numBytesRcvd < 0) {
        DieWithSystemMessage("recv() failed");
    }

    while (numBytesRcvd > 0) {
        ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
        if (numBytesSent < 0) {
            DieWithSystemMessage("send() failed");
        } else if (numBytesSent != numBytesRcvd) {
            DieWithUserMessage("send()", "sent unexpected number of bytes");
        }

        numBytesRcvd = recv(clntSocket, buffer, BUFSIZ, 0);
        if (numBytesRcvd < 0) {
            DieWithSystemMessage("recv() failed");
        }
    }

    close(clntSocket);
}

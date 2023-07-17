#include "httpServer.h"
using std::string;
using std::function;
#include "Practical.h"
#include <arpa/inet.h>
#include <vector>
using std::vector;
#include <sstream>
using std::istringstream;
using std::getline;
#include <exception>
using std::runtime_error;
using std::exception;
#include <iostream>
using std::cout;

using namespace http;

// Util
vector<string> splitString(const string& input, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(input);
    
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

struct ThreadArgs {
    int clntSock;
    void (http::Server::*handleClient)(int);
    Server *server;

    ThreadArgs(int clntSock, void (http::Server::*handleClient)(int), Server *server) :
        clntSock(clntSock), handleClient(handleClient), server(server) {}
};

void Server::endpoint(string &e, function<struct Response(struct Request &)> f) {
    endpoints[e] = f;
}

void Server::endpoint(const char *e, function<struct Response(struct Request &)> f) {
    endpoints[string(e)] = f;
}

struct Request Server::parseRequest(string request) {
    struct Request Request;

    vector<string> tokens = splitString(request, ' ');

    // Basic Error Checking; Should also check if valid method and version
    if(tokens.size() < 3) {
        throw runtime_error("Bad Request");
    }

    Request.method = tokens[0];
    Request.path = tokens[1];
    Request.version = tokens[2];

    return Request;
}

void Server::handleClient(int clntSock) {
    // Assuming we are getting HTTP requests from clntSock

    char buffer[BUFFERSIZE];

    ssize_t numBytesRcvd = recv(clntSock, buffer, BUFFERSIZE - 1, 0);
    if ( numBytesRcvd < 0 ) {
        printf("recv() failed for client %d\n", clntSock);
        return;
    }
    buffer[numBytesRcvd] = '\0';

    string request(buffer);
    
    struct Request Request;

    try {
        Request = parseRequest(request);
                
        if (endpoints.count(Request.path) == 1) {
            struct Response response = endpoints[Request.path](Request);

            send(clntSock, response.res.c_str(), response.res.length(), 0);
        } else {
            throw runtime_error("Bad endpoint");
        }

        std::cout << "Handled " << request << '\n';
    } catch (const exception &e) {
        printf("%s\n", e.what());

        const char *msg = "HTTP/1.1 400 Bad Request\n";

        send(clntSock, msg, strlen(msg), 0);
    }
    
    close(clntSock);
}

void *handleThread(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *) arg;

    int clntSock = args->clntSock;
    void (http::Server::*handleClient)(int) = args->handleClient;
    Server *server = args->server;

    (server->*handleClient)(clntSock);

    return NULL;
}

void Server::run(int servPort) {
    // Create socket
    servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( servSock < 0 ) {
        DieWithSystemMessage("socket() failed");
    }

    // Bind socket
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(servPort);
    if ( bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0 ) {
        DieWithSystemMessage("bind() failed");
    }

    // Listen socket
    if ( listen(servSock, MAXPENDING) < 0 ) {
        DieWithSystemMessage("listen() failed");
    }

    // Accept socket
    for(;;) {
        struct sockaddr_in clntAddr;
        socklen_t clntAddrLen = sizeof(clntAddr);

        int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
        if ( clntSock < 0 ) {
            DieWithSystemMessage("accept() failed");
        }

        char clntName[INET_ADDRSTRLEN];
        if ( inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr,
                        clntName, sizeof(clntName)) != NULL ) {
            printf("Handling client %s/%d %d\n", clntName, ntohs(clntAddr.sin_port), clntSock);
        } else {
            printf("Unable to get client address\n");
        }

        pthread_t thread;
        struct ThreadArgs args {clntSock, &Server::handleClient, this};
        int result = pthread_create(&thread, NULL, handleThread, (void *) &args);
        if (result != 0) {
            printf("Unable to create thread for request\n");
        }
    }
}

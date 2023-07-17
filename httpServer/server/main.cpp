#include <fstream>
using std::ifstream;
#include <string>
using std::string;
using std::istreambuf_iterator;
#include <iostream>
using std::cerr;
#include "../include/Practical.h"

#include "../include/httpServer.h"
using http::Server;
using http::Request;
using http::Response;

string readFileToString(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Failed to open file: " << filename << '\n';
        return "";
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    return content;
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        DieWithUserMessage("Parameters", "<Server Port>");
    }
    int servPort = atoi(argv[1]);

    Server server;

    server.endpoint("/hello-world", [](struct Request &request) {
        struct Response response(
            readFileToString("pages/hello-world.html")
        );

        return response;
    });

    server.endpoint("/page-one", [](struct Request &request) {
        struct Response response(
            readFileToString("pages/page-one.html")
        );

        return response;
    });


    server.endpoint("/page-two", [](struct Request &request) {
        struct Response response(
            readFileToString("pages/page-two.html")
        );

        return response;
    });


    server.run(servPort);

    return 0;
}

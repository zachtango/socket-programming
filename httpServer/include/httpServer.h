#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <unordered_map>

namespace http {
    // https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
    // Simplified HTTP Request
    struct Request {
        std::string method;
        std::string path;
        std::string version;
    };
    // Simplified HTTP Response
    struct Response {
        std::string res;

        Response(const std::string &body) {
            res = "HTTP/1.1 200 OK\n"
                    "Content-Type: text/html\n"
                    "Content-Length: " + std::to_string(body.length()) + "\n\n" +
                    body;
        }
    };

    class Server {
    private:
        const int MAXPENDING = 5;
        constexpr static int BUFFERSIZE = 30000;
        int servSock;
        struct sockaddr_in servAddr;

        std::unordered_map<std::string, std::function<struct Response(struct Request &)>> endpoints;

        void handleClient(int clntSock);
        struct Request parseRequest(std::string request);

    public:
        void run(int servPort);

        void endpoint(std::string &e, std::function<struct Response(struct Request &)> f);
        void endpoint(const char *e, std::function<struct Response(struct Request &)> f);

        ~Server() {
            close(servSock);
        }
    };

}


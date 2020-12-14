
#include "icore.h"
#include "httpserver.h"
#include "tcp.h"
#include <iostream>

int main(int argc, char *argv[]) {
    using namespace std;
    std::string ip = "192.168.122.1";
    int32_t port = 56789;
    HTTPServer * httpserver = new HTTPServer(ip, port);
    if (!httpserver->launch()) {
        return 0;
    }
    while (true) {
        httpserver->run();
    }
    
    return 0;
}
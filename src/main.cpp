
#include "icore.h"
#include "httpserver.h"
#include "tcp.h"
#include <iostream>

int main(int argc, char *argv[]) {
    using namespace std;
    cout << sizeof(TCP) << endl;

    std::string ip = "127.0.0.1";
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
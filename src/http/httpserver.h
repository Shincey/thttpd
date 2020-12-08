#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include "icore.h"
#include "httpsession.h"
#include "tcp.h"

class HTTPServer {
public:
    HTTPServer(std::string ip, int32_t port) : _listen_ip(ip), _listen_port(port) {}

    bool launch();

    void run();

private:
    std::string _listen_ip;
    int32_t _listen_port;
    Listener *_listener;
};

#endif // __HTTPSERVER_H__
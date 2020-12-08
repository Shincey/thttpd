#include "httpsession.h"


HTTPSession::HTTPSession() {}
HTTPSession::HTTPSession(const std::string ip, const int32_t port) : _ip(ip), _port(port) {}


int32_t HTTPSession::on_recv(const void* data, int32_t len) {
    char tmp[1024*1024];
    snprintf(tmp, len, "%s", (const char *)data);
    printf("on_recv: %s\n", tmp);

    return len;
}

void HTTPSession::on_connect() {

}

void HTTPSession::on_disconnect() {

}

void HTTPSession::on_failedconnect() {

}

void HTTPSession::send(const void *data, const int32_t len) {

}

void HTTPSession::close() {

}

void HTTPSession::load() {

}

void HTTPSession::cache() {

}
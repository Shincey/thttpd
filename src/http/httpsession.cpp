#include "httpsession.h"

#include <string.h>

HTTPSession::HTTPSession() : icore::iSession("", -1) {}
HTTPSession::HTTPSession(const std::string ip, const int32_t port) : icore::iSession(ip, port) {}


int32_t HTTPSession::on_recv(const void* data, int32_t len) {
    char tmp[1024*1024];
    snprintf(tmp, len, "%s", (const char *)data);
    printf("on_recv: %s\n", tmp);
    const char * r = "recieved\n";
    send((const void *)r, strlen(r));
    return len;
}

void HTTPSession::on_connect() {
    printf("on_connect:\n");
}

void HTTPSession::on_disconnect() {
    printf("on_disconnect:\n");
}

void HTTPSession::on_failedconnect() {
    printf("on_failedconnect:\n");
}

void HTTPSession::send(const void *data, const int32_t len) {
    if (this && _pipe) {
        _pipe->send(data, len);
    }
}

void HTTPSession::close() {

}

void HTTPSession::load() {

}

void HTTPSession::cache() {

}

void HTTPSession::malloc_pipe(icore::iPipe *pipe) {
    _pipe = pipe;
}
#include "httpserver.h"

#include "tcp.h"
#include "global.h"
#include "defines.h"

#include <iostream>

extern int32_t g_epoller_fd;

bool HTTPServer::launch() {
    if (g_epoller_fd = epoll_create(EPOLLER_DESC_COUNT); -1 == g_epoller_fd) {
        return false;
    }
    ;
    if (Listener *listen = new Listener(_listen_ip, _listen_port, 1024, 1024); listen == nullptr) { 
        return false; 
    } else {
       _listener = listen;
    }
    return true;
}


void HTTPServer::run() {
    static struct epoll_event evs[512];
    int64_t tick = tool::microseconds();
    do {
        errno = 0;
        int32_t count = 0;
        if (count = epoll_wait(g_epoller_fd, evs, 512, 1); 0 == count) { return; }
        if (-1 == count) { return; }
        for (int32_t i = 0; i < count; ++i) {
            Association *association = (Association *)evs[i].data.ptr;
            association->_handler->on_epoll_event(association, association->_type, evs[i]);
        }
    } while (tool::microseconds() - tick < 1000);
}
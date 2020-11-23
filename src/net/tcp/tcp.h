#ifndef __TCP_H__
#define __TCP_H__

#include "icore.h"
#include "common.h"
#include "buffer.h"

#define kMAX_BUF_SIZE 1024

class TCP : public icore::iPipe, public iEpollEventHandler {
public:

    TCP();
    TCP(const std::string ip, const int32_t port, const int32_t rsize, const int32_t ssize);

    virtual void cache();
    virtual void load();
    virtual void close();
    virtual void send(const void* data, const int32_t len);

    virtual void on_recv(const char* data, int32_t len) = 0;
    virtual void on_connect() = 0;
    virtual void on_disconnect() = 0;
    virtual void on_failedconnect() = 0;

    virtual void on_epoll_event(Association * association, const eEpollEventType type, const struct epoll_event &event);


private:
    friend void tcp_connect_handler(TCP * tcp, Association * association, const eEpollEventType type, const struct epoll_event &event);

    friend void tcp_io_handler(TCP * tcp, Association * association, const eEpollEventType type, const struct epoll_event &event);

private:
    int32_t _sockfd;

    Association *_association;

    buffer _rbuf;
    buffer _sbuf;
};

#endif //__TCP_H__
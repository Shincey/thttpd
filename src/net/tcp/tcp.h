#ifndef __TCP_H__
#define __TCP_H__

#include "icore.h"
#include "common.h"
#include "buffer.h"
#include "pool.h"

#define kMAX_BUF_SIZE 1024

class Listener : public icore::iPipe, public iEpollEventHandler {
public:
    Listener(const std::string ip, const int32_t port, const int32_t rsize, const int32_t ssize);

    virtual void cache();
    virtual void load();
    virtual void close();
    virtual void send(const void* data, const int32_t len);

    virtual int32_t on_recv(const void* data, int32_t len);
    virtual void on_connect();
    virtual void on_disconnect();
    virtual void on_failedconnect();

    virtual void on_epoll_event(Association * association, const eEpollEventType type, const struct epoll_event &event);

private:
    Association _association;
};

class TCP : public icore::iPipe, public iEpollEventHandler {
public:

    TCP();
    TCP(const std::string ip, const int32_t port, const int32_t fd, const int32_t rsize, const int32_t ssize);

    virtual void cache();
    virtual void load();
    virtual void close();
    virtual void send(const void* data, const int32_t len);

    virtual int32_t on_recv(const void* data, int32_t len);
    virtual void on_connect();
    virtual void on_disconnect();
    virtual void on_failedconnect();

    virtual void on_epoll_event(Association * association, const eEpollEventType type, const struct epoll_event &event);


private:
    friend void tcp_connect_handler(TCP * tcp, Association * association, const eEpollEventType type, const struct epoll_event &event);

    friend void tcp_io_handler(TCP * tcp, Association * association, const eEpollEventType type, const struct epoll_event &event);


private:
    friend Listener;
    friend pool<TCP>;

private:
    Association _association;
    buffer _rbuf;
    buffer _sbuf;
    bool _connecting;
};

#endif //__TCP_H__

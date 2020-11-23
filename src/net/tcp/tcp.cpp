#include "tcp.h"

#include <sys/socket.h>

TCP::TCP() : _sockfd(-1), 
             _association(nullptr), 
             _rbuf(kMAX_BUF_SIZE), 
             _sbuf(kMAX_BUF_SIZE) {}

TCP::TCP(const std::string ip, const int32_t port, const int32_t rsize, const int32_t ssize) : iPipe(ip, port),
         _association(nullptr),
         _rbuf(rsize),
         _sbuf(ssize) {}


void TCP::cache() {
    
}
void TCP::load() {
    
}
void TCP::close() {
    if (_sockfd != -1) {
        ::close(_sockfd);
        _sockfd = -1;
        on_disconnect();
    }
}
void TCP::send(const void* data, const int32_t len) {
    if (_sockfd == -1) { return; }
    {
        struct epoll_event ev;
        ev.data.ptr = (void*)&_association;
        ev.events = EPOLLIN | EPOLLOUT;
        epoll_ctl(1, EPOLL_CTL_MOD, _sockfd, &ev);
    }
}

void TCP::on_epoll_event(Association * association, const eEpollEventType type, const struct epoll_event &event) {
    switch (type) {
        case eEpollEventType::eConnect : {
            tcp_connect_handler(this, association, type, event);
            break;
        }
        case eEpollEventType::eIO : {
            tcp_io_handler(this, association, type, event);
            break;
        }
        default : {
            break;
        }
    }
}

void tcp_connect_handler(TCP * tcp, Association * association, const eEpollEventType type, const struct epoll_event &event) {
    auto clear = [&] () {
        ::close(tcp->_sockfd);
        tcp->_sockfd = -1;
    };

    if (-1 == settcpnodelay(tcp->_sockfd)) {
        clear();
        return;
    }
    epoll_ctl(1, EPOLL_CTL_DEL, tcp->_sockfd, nullptr);
    if (event.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
        clear();
        return;
    }
    if (event.events & EPOLLOUT) {
        struct epoll_event ev;
        ev.data.ptr = (void *)&tcp->_association;
        ev.events = EPOLLIN;
        if (-1 == epoll_ctl(1, EPOLL_CTL_ADD, tcp->_sockfd, &ev)) {
            clear();
            return;
        }
        tcp->on_connect();
    } else {
        clear();
        return;
    }
}

void tcp_io_handler(TCP * tcp, Association * association, const eEpollEventType type, const struct epoll_event &event) {

}
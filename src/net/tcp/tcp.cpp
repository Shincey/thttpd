#include "tcp.h"

#include "global.h"

#include "httpsession.h"

#include <iostream>
#include <sys/socket.h>

class TCP;

extern pool<TCP> g_tcp_pool;

extern int32_t g_epoller_fd;


Listener::Listener(const std::string ip, const int32_t port, const int32_t rsize, const int32_t ssize)
    : iPipe(ip, port),
      _association(eEpollEventType::eAccept, this) {
            
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    addr.sin_port = htons(port);
    
    if (-1 == (_fd = ::socket(AF_INET, SOCK_STREAM, 0)) ||
        -1 == setreuse(_fd) ||
        -1 == setnonblocking(_fd) ||
        -1 == ::bind(_fd, (struct sockaddr *)&addr, sizeof(addr)) ||
        -1 == ::listen(_fd, 200)) {
        _fd = -1;
        return;
    }
    epoll_event ev;
    ev.data.ptr = (void *)&_association;
    ev.events = EPOLLIN;
    if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_ADD, _fd, &ev)) {
        // do something...
        return;
    }
}

void Listener::cache() {}
void Listener::load() {}
void Listener::close() {}
void Listener::send(const void* data, const int32_t len) {}

int32_t Listener::on_recv(const void* data, int32_t len) {}
void Listener::on_connect() {}
void Listener::on_disconnect() {}
void Listener::on_failedconnect() {}

void Listener::on_epoll_event(Association * association, const eEpollEventType type, const struct epoll_event &ev) {
    tassert(eEpollEventType::eAccept == type, "not accepter");
    if (ev.events & EPOLLIN) {
        struct sockaddr_in addr_in;
        socklen_t len = sizeof(addr_in);
        int32_t sockfd = -1;
        int32_t time = 0;
        while ((sockfd = ::accept(_fd, (struct sockaddr *)&addr_in, &len)) >= 0 && time++ <= 256) {
            if (-1 == setnonblocking(sockfd) ||
                -1 == settcpnodelay(sockfd) ||
                -1 == setreuse(sockfd) ||
                -1 == setsocksendbuff(sockfd, 0) ||
                -1 == setsockrecvbuff(sockfd, 0)
            ) {
                ::close(sockfd);
                sockfd = -1;
                continue;
            }

            struct sockaddr_in sai;
            socklen_t len = sizeof(sai);
            ::getpeername(sockfd, (struct sockaddr *)&sai, (socklen_t *)&len);
            const std::string ip = ::inet_ntoa(sai.sin_addr);
            const int32_t port = ::ntohs(sai.sin_port);

            TCP *tcp = create_from_pool(g_tcp_pool, ip, port, sockfd, 1024, 1024);
            struct epoll_event ev;
            ev.data.ptr = (void *)&tcp->_association;
            ev.events = EPOLLIN;
            if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_ADD, sockfd, &ev)) {
                ::close(sockfd);
                sockfd = -1;
                recover_to_pool(g_tcp_pool, tcp);
                tcp = nullptr;
                continue;
            }
            tcp->_session = create_from_pool(g_http_session_pool, ip, port);
            tcp->_session->malloc_pipe(tcp);
            tcp->_connecting = true;
            tcp->on_connect();
        }
    } else {
        // do something
    }
}



TCP::TCP() : _association(eEpollEventType::eIO, this), 
             _rbuf(kMAX_BUF_SIZE), 
             _sbuf(kMAX_BUF_SIZE),
             _connecting(false) {}

TCP::TCP(const std::string ip, const int32_t port, const int32_t fd, const int32_t rsize, const int32_t ssize) : iPipe(ip, port, fd),
         _association(eEpollEventType::eIO, this),
         _rbuf(rsize),
         _sbuf(ssize),
         _connecting(false) {}


void TCP::cache() {
    
}
void TCP::load() {
    
}
void TCP::close() {
    if (_fd != -1) {
        _connecting = false;
        epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, _fd, nullptr);
        ::close(_fd);
        _fd = -1;
        if (_session) {
            _session->on_disconnect();
        }
        recover_to_pool(g_http_session_pool, (HTTPSession *)this->_session);
        recover_to_pool(g_tcp_pool, this);
    }
}

void TCP::send(const void* data, const int32_t len) {
    printf("in tcp send\n");
    if (!_connecting) {
        return;
    }
    if (_sbuf.in(data, len)) {
        struct epoll_event ev;
        ev.data.ptr = (void*)&_association;
        ev.events = EPOLLIN | EPOLLOUT;
        epoll_ctl(g_epoller_fd, EPOLL_CTL_MOD, _fd, &ev);
    } else {
        close();
    }
}

int32_t TCP::on_recv(const void* data, int32_t len) {
    if (_session) {
        return _session->on_recv(data, len);
    }
    return 0;
}

void TCP::on_connect() {
    if (_session) {
        _session->on_connect();
    }
    std::cout << "tcp connection established." << std::endl;
    std::cout << "peer ip: " << this->_addr._ip << std::endl;
    std::cout << "peer port: " << this->_addr._port << std::endl;
    std::cout << "socket fd: " << this->_fd << std::endl;
}

void TCP::on_disconnect() {
    if (_session) {
        _session->on_disconnect();
    }
}

void TCP::on_failedconnect() {
    if (_session) {
        _session->on_failedconnect();
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
        ::close(tcp->_fd);
        tcp->_fd = -1;
        tcp->_session ? (void)0 : tcp->on_failedconnect();
        recover_to_pool(g_tcp_pool, tcp);
    };

    if (-1 == settcpnodelay(tcp->_fd)) {
        clear();
        return;
    }

    epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, tcp->_fd, nullptr);
    if (event.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
        clear();
        return;
    }

    if (event.events & EPOLLOUT) {
        struct epoll_event ev;
        ev.data.ptr = (void *)&tcp->_association;
        ev.events = EPOLLIN;
        if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_ADD, tcp->_fd, &ev)) {
            clear();
            return;
        }
        tcp->on_connect();
    } else {
        clear();
        return;
    }
}

void tcp_io_handler(TCP * tcp, Association * association, const eEpollEventType type, const struct epoll_event &ev) {
    auto error_handler = [&] () {
        tassert(tcp, "tcp is nullptr");
        if (!tcp) return;
        ::close(tcp->_fd);
        tcp->_fd = -1;
        tcp->on_disconnect();
        recover_to_pool(g_http_session_pool, (HTTPSession *)tcp->_session);
        recover_to_pool(g_tcp_pool, tcp);
    };
    if (ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
        tcp->close();
    } else {
        if (ev.events & EPOLLIN) {
            static char tmp[2048];
            if (-1 != tcp->_fd) {
                const int64_t tick = tool::microseconds();
                do {
                    int32_t len = ::recv(tcp->_fd, tmp, sizeof(tmp), 0);
                    if (len > 0) {
                        if (tcp->_rbuf.in(tmp, len)) {
                            while (tcp->_rbuf.length() > 0) {
                                int32_t use = tcp->on_recv(tcp->_rbuf.data(), tcp->_rbuf.length());
                                if (!tcp->_connecting) {
                                    // epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, tcp->_fd, nullptr);
                                    // error_handler();
                                    tcp->close();
                                    return;
                                }
                                if (use <= 0) {
                                    return;
                                }
                                tcp->_rbuf.out(use);
                            }
                        } else {
                            tcp->close();
                            return;
                        }
                    } else if (len < 0 && EAGAIN == errno) {
                        return;
                    } else {
                        tcp->close();
                        return;
                    }
                } while (tcp->_connecting && tool::microseconds() - tick <= 1000);
            }
        }

        if (ev.events & EPOLLOUT) {
            int32_t len = ::send(tcp->_fd, tcp->_sbuf.data(), tcp->_sbuf.length(), 0);
            if (len > 0) {
                tcp->_sbuf.out(len);
                if (tcp->_sbuf.length() == 0) {
                    struct epoll_event ev;
                    ev.data.ptr = (void *)&tcp->_association;
                    ev.events = EPOLLIN;
                    if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_MOD, tcp->_fd, &ev)) {
                        //epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, tcp->_fd, nullptr);
                        //error_handler();
                        tcp->close();
                        return;
                    }
                } else if (len <= 0 && EAGAIN != errno) {
                    //epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, tcp->_fd, nullptr);
                    //error_handler();
                    tcp->close();
                }
            }
        }
    }
}

#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

enum eEpollEventType {
    eAccept,
    eConnect,
    eIO,
};

class iEpollEventHandler;

class Association {
public:
    Association(const eEpollEventType type, iEpollEventHandler * const handler) : _type(type), _handler(handler) {}
    const eEpollEventType _type;
    iEpollEventHandler * _handler;
};


class iEpollEventHandler {
public:
    virtual ~iEpollEventHandler() {}
    virtual void on_epoll_event(Association * association, const eEpollEventType type, const struct epoll_event &event) = 0;
};





#ifdef __cplusplus
extern "C" {
#endif
    inline int32_t setnonblocking(int sockfd) {
        return fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
    }

    inline int32_t setreuse(const int32_t fd) {
        int val = 1l;
        return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&val, sizeof(val));
    }

    inline int32_t settcpnodelay(const int32_t fd) {
        int val = 1l;
        return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char *)&val, sizeof(val));
    }

    inline int32_t settcpquickack(const int32_t fd) {
        int val = 1l;
        return setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, (const char *)&val, sizeof(val));
    }

    inline int32_t setsocksendbuff(int sockfd, int size) {
        return setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (const char *)&size, sizeof(size));
    }

    inline int32_t setsockrecvbuff(int sockfd, int size) {
        return setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (const char *)&size, sizeof(size));
    }

    inline int32_t setmaxopenfiles(const int32_t size) {
        struct rlimit rt;
        rt.rlim_max = rt.rlim_cur = size;
        if (-1 == setrlimit(RLIMIT_NOFILE, &rt)) return -1;
        return 0;
    }

    inline int32_t setstacksize(const int32_t size) {
        struct rlimit rt;
        rt.rlim_max = rt.rlim_cur = size * 1024;
        if (-1 == setrlimit(RLIMIT_STACK, &rt)) return  -1;
        return 0;
    }

    inline bool get_ip_by_host(const char *__host, std::string &__ip) {
        struct hostent *hp;
        if ((hp = gethostbyname(__host)) == NULL) return false;
        __ip = inet_ntoa(*(struct in_addr*)hp->h_addr);
        return true;
    }

#ifdef __cplusplus
}
#endif

#endif // __COMMON_H__
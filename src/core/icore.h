#ifndef __ICORE_H__
#define __ICORE_H__

#include <string>

namespace icore {

    class iSocket{
    public:
        iSocket() : _ip(""), _port(0) {}
        iSocket(std::string ip, const int32_t port) : _ip(ip), _port(port) {}

        std::string _ip;
        int32_t _port;
    };

    class iSession {
    public:
        virtual ~iSession() {}
        virtual int32_t on_recv(const void* data, int32_t len) = 0;
        virtual void on_connect() = 0;
        virtual void on_disconnect() = 0;
        virtual void on_failedconnect() = 0;
        virtual void send(const void *data, const int32_t len) = 0;
        virtual void close() = 0;
        virtual void load() = 0;
        virtual void cache() = 0;
    private:
        std::string _ip;
        int32_t _port;
    };

    class iPipe {
    public:
        iPipe() {}
        iPipe(const std::string ip, const int32_t port) : _addr(ip, port), _fd(-1), _session(nullptr) {}
        iPipe(const std::string ip, const int32_t port, const int32_t fd) : _addr(ip, port), _fd(fd), _session(nullptr) {}
        iPipe(const std::string ip, const int32_t port, const int32_t fd, iSession *session) : _addr(ip, port), _fd(fd), _session(session) {}
        virtual ~iPipe() {}
        virtual void cache() = 0;
        virtual void load() = 0;
        virtual void close() = 0;
        virtual void send(const void* data, const int32_t len) = 0;

        virtual int32_t on_recv(const void* data, int32_t len) = 0;
        virtual void on_connect() = 0;
        virtual void on_disconnect() = 0;
        virtual void on_failedconnect() = 0;
    protected:
        iSocket _addr;
        int32_t _fd;
        iSession * _session;
    };


    class iCore {
    public:
        virtual ~iCore() {}

        virtual bool launch() = 0;
        virtual bool launch_tcp_server() = 0;

        virtual iSession * malloc_session(const std::string ip, const int32_t port) = 0;

        virtual void loop() = 0;
    };

}

#endif // __ICORE_H__
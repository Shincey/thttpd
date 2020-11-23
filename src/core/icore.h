#ifndef __ICORE_H__
#define __ICORE_H__

#include <string>

namespace icore {

    class iSocket{
    public:
        iSocket() : _ip(""), _port(0) {}
        iSocket(std::string ip, const int32_t port) : _ip(ip), _port(port) {}
    protected:
        std::string _ip;
        int32_t _port;
    };

    class iPipe {
    public:
        iPipe() {}
        iPipe(const std::string ip, const int32_t port) : _addr(ip, port) {}
        virtual ~iPipe() {}
        virtual void cache() = 0;
        virtual void load() = 0;
        virtual void close() = 0;
        virtual void send(const void* data, const int32_t len) = 0;

        virtual void on_recv(const char* data, int32_t len) = 0;
        virtual void on_connect() = 0;
        virtual void on_disconnect() = 0;
        virtual void on_failedconnect() = 0;
    protected:
        iSocket _addr;
    };

    class iSession {
    public:
        virtual ~iSession() {}
        iSession() {}
        virtual void on_recv(const char* data, int32_t len) {
            if (this && _pipe) { _pipe->on_recv(data, len); }
        }
        virtual void on_connect() {
            if (this && _pipe) { _pipe->on_connect(); }
        }
        virtual void on_disconnect() {
            if (this && _pipe) { _pipe->on_disconnect(); }
        }
        virtual void on_failedconnect() {
            if (this && _pipe) { _pipe->on_failedconnect(); }
        }

        virtual void send(const void *data, const int32_t len) const {
            if (this && _pipe) { _pipe->send(data, len); }
        }
        virtual void close() const {
            if (this && _pipe) { _pipe->close(); }
        }
        virtual void load() const {
            if (this && _pipe) { _pipe->load(); }
        }
        virtual void cache() const {
            if (this && _pipe) { _pipe->cache(); }
        }
    protected:
        iPipe *_pipe;
    };

}

#endif // __ICORE_H__
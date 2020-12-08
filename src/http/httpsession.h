#ifndef __HTTPSESSION_H__
#define __HTTPSESSION_H__

#include "icore.h"

class HTTPSession : public icore::iSession {
public:
        HTTPSession();
        HTTPSession(const std::string ip, const int32_t port);
        virtual int32_t on_recv(const void* data, int32_t len);
        virtual void on_connect();
        virtual void on_disconnect();
        virtual void on_failedconnect();
        virtual void send(const void *data, const int32_t len);
        virtual void close();
        virtual void load();
        virtual void cache();
private:
        std::string _ip;
        int32_t _port;
};

#endif // __HTTPSESSION_H__
#ifndef __TCP_H__
#define __TCP_H__

#include "icore.h"

class TCP : public icore::iPipe {
public:
    virtual void cache();
    virtual void load();
    virtual void close();
    virtual void send(const void* data, const int32_t len);

private:
    int32_t _sockfd;

    const int32_t _send_size;
    const int32_t _recv_size;
};

#endif //__TCP_H__

#ifndef __CORE_H__
#define __CORE_H__

#include "icore.h"

class Core : public icore::iCore {
public:
    virtual bool launch();

    virtual bool launch_tcp_server();

    icore::iSession * malloc_session(const std::string ip, const int32_t port);

    //virtual void loop();
};

#endif // __CORE_H__
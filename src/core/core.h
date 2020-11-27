#ifndef __CORE_H__
#define __CORE_H__

#include "icore.h"

class Core : public icore::iCore {
public:
    virtual bool launch();

    virtual bool launch_tcp_server();
    
    virtual void loop();
};

#endif // __CORE_H__
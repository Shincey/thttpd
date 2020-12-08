#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdint.h>

#include "pool.h"
#include "tcp.h"
#include "httpsession.h"

extern int32_t g_epoller_fd;

extern pool<TCP> g_tcp_pool;

extern pool<HTTPSession> g_http_session_pool;


#endif // __GLOBAL_H__
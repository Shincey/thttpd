#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdint.h>

#include "pool.h"

int32_t g_epoller_fd = -1;

class TCP;
pool<TCP> g_tcp_pool;




#endif // __GLOBAL_H__
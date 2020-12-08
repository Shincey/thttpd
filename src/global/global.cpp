#include "global.h"

int32_t g_epoller_fd = -1;

pool<TCP> g_tcp_pool;

pool<HTTPSession> g_http_session_pool;
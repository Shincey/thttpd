#include "core.h"
#include "common.h"
#include "tools.h"
#include "global.h"
#include "defines.h"

#include "httpsession.h"

bool Core::launch() {
    // if (g_epoller_fd = epoll_create(EPOLLER_DESC_COUNT); -1 == g_epoller_fd) {
    //     return false;
    // }
    // return true;
}

bool Core::launch_tcp_server() {

}


// void Core::loop() {
//     static struct epoll_event evs[512];
//     int64_t tick = tool::microseconds();
//     do {
//         int32_t count = 0;
//         if (count = epoll_wait(g_epoller_fd, evs, 512, 10); 0 == count || -1 == count) {
//             return;
//         }
//         for (int i = 0; i < count; ++i) {
//             struct Association *association = (struct Association *)evs[i].data.ptr;
//             association->_handler->on_epoll_event(association, association->_type, evs[i]);
//         }
//     } while (tool::microseconds() - tick < 1000);
// }



icore::iSession * Core::malloc_session(const std::string ip, const int32_t port) {
    return create_from_pool(g_http_session_pool, ip, port);
}
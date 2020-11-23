#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <cmath>
#include <chrono>
#include <string>
#include <vector>
#include <cstring>

#include "tassert.h"

namespace tool {
    
    inline void smemset(void *__restrict dest, size_t max, int val, size_t n) {
        tassert(n <= max, "over flow");
        memset(dest, val, (max >=n) ? (n) : (max));
    }

    inline void smemcpy(void *__restrict dest, size_t max, const void *src, size_t n) {
        tassert(n <= max, "over flow");
        memcpy(dest, src, (max >=n) ? (n) : (max));
    }


    inline bool slocaltime(struct tm &ts, const time_t &tt) {
        const struct tm *pt = localtime(&tt);
        if (nullptr == pt) { return false; }
        smemcpy(&ts, sizeof(ts), pt, sizeof(ts));
    }

    inline uint64_t milliseconds() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    inline uint64_t microseconds() {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    inline std::string now(const char *format = "%4d-%02d-%02d %02d:%02d:%02d") {
        char strtime[64] = {0};
        auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct tm ts;
        slocaltime(ts, tt);
        snprintf(strtime, sizeof(strtime), format, (int)ts.tm_year + 1900, (int)ts.tm_mon + 1, (int)ts.tm_mday, (int)ts.tm_hour, (int)ts.tm_min, (int)ts.tm_sec);
        return strtime;
    }

    /**
     * Convert timestamp to readable time representation
     * @param timestamp : timestamp in milliseconds
     * @param format : time format to convert
     * @return : returns the formatted time.
     */
    inline std::string time(const int64_t timestamp , const char *format = "%4d-%02d-%02d %02d:%02d:%02d") {
        char strtime[128];
        auto ms = std::chrono::milliseconds(timestamp);
        auto tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(ms);
        auto tt = std::chrono::system_clock::to_time_t(tp);
        struct tm ts;
        slocaltime(ts, tt);
        snprintf(strtime, sizeof(strtime), format,
                    (int)ts.tm_year+1900,
                    (int)ts.tm_mon+1,
                    (int)ts.tm_mday,
                    (int)ts.tm_hour,
                    (int)ts.tm_min,
                    (int)ts.tm_sec);
        return strtime;
    }

}

#endif // __TOOLS_H__
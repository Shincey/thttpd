#ifndef __TASSERT_H__
#define __TASSERT_H__

#include <stdio.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif
    void tassertion(const char *file, int line, const char *function, const char *debug);
#ifdef __cplusplus
};
#endif





#ifdef DEBUG
#define tassert(p, format, ...) {\
    char debug[4096] = {0}; \
    snprintf(debug, sizeof(debug), format, ##__VA_ARGS__); \
    ((p) ? (void)0 : (void)tassertion(__FILE__, __LINE__, __FUNCTION__, debug)); \
}
#else
#define tassert
#endif




#endif // __TASSERT_H__
#include "tassert.h"


#ifdef __cplusplus
extern "C" {
#endif
    void tassertion(const char *file, int line, const char *function, const char *debug) {
#ifdef DEBUG
        fflush(stdout);
        printf("ASSERT POS : %s:%d @function:%s\nASSERT INF : %s\n", file, line, function, debug);
        fflush(stdout);
        assert(false);
#endif // DEBUG
    }
#ifdef __cplusplus
};
#endif
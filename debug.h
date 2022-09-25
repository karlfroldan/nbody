#ifndef DEBUG_H
#define DEBUG_H
#ifdef DEBUG
#include <assert.h> 

#define DPRINTF(format, ...) \
    do { \
        fprintf(stderr, "[LOG] %s::%s> "format, __FILE__, __FUNCTION__, ##__VA_ARGS__); \
    } while (0)
#else 
/* If we are not on debug mode, do nothing */
#define DPRINTF(format, ...) ((void*) 0)
#endif /* DEBUG  */
#endif /* DEBUG_H */

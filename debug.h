#ifndef DEBUG_H
#define DEBUG_H
#ifdef DEBUG
#include <assert.h> 

#include "quadtree.h"

#define DPRINTF(format, ...) \
    do { \
        fprintf(stderr, "[LOG] %s::%s> "format, __FILE__, __FUNCTION__, ##__VA_ARGS__); \
    } while (0)

int __show_quadtree(quadtree_t*);
int __quadtree_elems_aux(quadtree_t*);

#define LIST_QUADTREE(qt) __quadtree_elems_aux(qt)
#define SHOW_QUADTREE(qt) __show_quadtree(qt)
#else 
/* If we are not on debug mode, do nothing */
#define DPRINTF(format, ...) ((void*) 0)
#define SHOW_QUADTREE(qt)
#define LIST_QUADTREE(qt)
#endif /* DEBUG  */
#endif /* DEBUG_H */

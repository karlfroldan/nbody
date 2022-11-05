#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPACES(n) \
    do {\
        for (int i = 0; i < (n) * 2; ++i) {\
            putchar(' ');\
        }\
    } while(0)

#define NODETYPE_STR(type) \
    NODE_INTERNAL == (type) ? "internal" : "external"

int __show_quadtree_aux(quadtree_t* qt, int spacing) {
    if (qt != NULL) {
        SPACES(spacing);
        printf("- %s (x: %lf, y: %lf, mass: %lf, type: %s)\n", 
            qt->node_type == NODE_INTERNAL ? "INTERNAL" : "Node", 
            qt->x_pos, qt->y_pos, qt->mass, NODETYPE_STR(qt->node_type));

        SPACES(spacing);
        printf("  Northwest\n");
        __show_quadtree_aux(qt->nw, spacing + 1);
        SPACES(spacing);
        printf("  Northeast\n");
        __show_quadtree_aux(qt->ne, spacing + 1);
        SPACES(spacing);
        printf("  Southwest\n");
        __show_quadtree_aux(qt->sw, spacing + 1);
        SPACES(spacing);
        printf("  Southeast\n");
        __show_quadtree_aux(qt->se, spacing + 1);
    } else {
        SPACES(spacing);
        printf("- Nothing here\n");
    }
    return 0;
}

int __show_quadtree(quadtree_t* qt) {
    printf("Showing quadtree\n");

    return __show_quadtree_aux(qt, 0);
}

int __quadtree_elems_aux(quadtree_t* qt) {
    if (qt == NULL) {
        return 0;
    }

    if (qt->node_type == NODE_EXTERNAL) {
        printf("position: (%lf, %lf)\n", qt->x_pos, qt->y_pos);
        printf("mass: (%lf, %lf)\n", qt->x_pos, qt->y_pos);
        printf("acceleration: (%lf, %lf)\n", qt->body->a_x, qt->body->a_y);
        printf("velocity: (%lf, %lf)\n", qt->body->v_x, qt->body->v_y);
        printf("=======================\n");
    }

    __quadtree_elems_aux(qt->nw);
    __quadtree_elems_aux(qt->ne);
    __quadtree_elems_aux(qt->sw);
    __quadtree_elems_aux(qt->se);

    return 0;
}


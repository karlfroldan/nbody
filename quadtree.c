#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

#include "quadtree.h"

#define INTERVALSIZE 4

#define INQUAD(xvar, yvar, its, quad) \
    xvar >= its->quad.xvar##_begin && xvar < its->quad.xvar##_end && \
    yvar >= its->quad.yvar##_begin && yvar < its->quad.yvar##_end

#define SETINTERVAL(its, quad, xbeg, xend, ybeg, yend) \
    do { \
        its->quad.x_begin = xbeg; \
        its->quad.x_end = xend; \
        its->quad.y_begin = ybeg; \
        its->quad.y_end = yend; \
    } while (0)

static double canvas_x = 1.0;
static double canvas_y = 1.0;

enum quadrant {
    QUADRANT_NE,
    QUADRANT_NW,
    QUADRANT_SE,
    QUADRANT_SW
};

/* To enable quadrant printing on debug */
#ifdef DEBUG 

struct quadrant_d {
    enum quadrant quadrant;
    char* desc;
};

static struct quadrant_d quad_map[] = {
    {QUADRANT_NE, "northeast"},
    {QUADRANT_NW, "northwest"},
    {QUADRANT_SE, "southeast"},
    {QUADRANT_SW, "southwest"},
};

char* quad_desc(enum quadrant q) {
    for (int i = 0; i < (int) (sizeof(quad_map) / sizeof(struct quadrant_d)); ++i) {
        if (q == quad_map[i].quadrant) {
            return quad_map[i].desc;
        }
    }

    return "disallowed";
}

#endif

/* The interval structure stores division by 2 of a 2D quadrant */
struct interval_pos {
    struct boundary ne;
    struct boundary nw;
    struct boundary se;
    struct boundary sw;
};

#ifdef DEBUG
#define QPRINT(quad) \
    do { \
        _qprint(quad);\
    } while(0)

void _qprint(struct interval_pos* q) {
    DPRINTF("Northwest\n");
    DPRINTF("(%.3lf, %.3lf) -- (%.3lf, %.3lf)\n",
        q->nw.x_begin, q->nw.y_begin, q->nw.x_end, q->nw.y_end);
    DPRINTF("Northeast\n");
    DPRINTF("(%.3lf, %.3lf) -- (%.3lf, %.3lf)\n",
        q->ne.x_begin, q->ne.y_begin, q->ne.x_end, q->ne.y_end);
    DPRINTF("Southwest\n");
    DPRINTF("(%.3lf, %.3lf) -- (%.3lf, %.3lf)\n",
        q->sw.x_begin, q->sw.y_begin, q->sw.x_end, q->sw.y_end);
    DPRINTF("Southeast\n");
    DPRINTF("(%.3lf, %.3lf) -- (%.3lf, %.3lf)\n",
        q->se.x_begin, q->se.y_begin, q->se.x_end, q->se.y_end);
}
#else
#define QPRINT(quad)
#endif /* DEBUG */


void calculate_ints(struct interval_pos* ints, struct boundary* base_int) {

    double x_half_len = (base_int->x_end - base_int->x_begin) / 2;
    double y_half_len = (base_int->y_end - base_int->y_begin) / 2;

    DPRINTF("base begin : (%.3lf, %.3lf), base end : (%.3lf, %.3lf)\n",
        base_int->x_begin, base_int->y_begin,
        base_int->x_end, base_int->y_end 
    );

    DPRINTF("half lengths: (%.3lf, %.3lf)\n", x_half_len, y_half_len);

    double x_half = base_int->x_begin + x_half_len;
    double y_half = base_int->y_begin + y_half_len;

    DPRINTF("halves: (%.3lf, %.3lf)\n", x_half, y_half);
    DPRINTF("========================\n");

    SETINTERVAL(ints, nw, base_int->x_begin, x_half, base_int->y_begin, y_half);
    SETINTERVAL(ints, ne, x_half, base_int->x_end, base_int->y_begin, y_half);
    SETINTERVAL(ints, sw, base_int->x_begin, x_half, y_half, base_int->y_end);
    SETINTERVAL(ints, se, x_half, base_int->x_end, y_half, base_int->y_end);
}

quadtree_t* qt_add_node(double x_pos, double y_pos, double mass,
    double x_begin, double x_end, double y_begin, double y_end) {
    quadtree_t* qt;

    if ((qt = malloc(sizeof(quadtree_t))) == NULL) {
        return NULL;
    }

    qt->x_pos = x_pos;
    qt->y_pos = y_pos;
    qt->mass = mass;
    qt->ne = NULL;
    qt->nw = NULL;
    qt->se = NULL;
    qt->sw = NULL;

    qt->boundary.x_begin = x_begin;
    qt->boundary.x_end   = x_end;
    qt->boundary.y_begin = y_begin;
    qt->boundary.y_end   = y_end;

    return qt;
}

quadtree_t* qt_add(quadtree_t* qt, double x, double y, double m) {
    struct boundary boundary = {
        .x_begin = 0.0,
        .x_end = canvas_x,
        .y_begin = 0.0,
        .y_end = canvas_y,
    };

    return __qt_add(qt, x, y, m, &boundary);
}


/* Find the quadrant of the given x_position and y_position */
enum quadrant find_quadrant(struct interval_pos* q, double x, double y) {
    DPRINTF("(%.2lf, %.2lf)\n", x, y);
    QPRINT(q);
    if (INQUAD(x, y, q, ne)) {
        return QUADRANT_NE;
    }

    if (INQUAD(x, y, q, nw)) {
        return QUADRANT_NW;
    }

    if (INQUAD(x, y, q, se)) {
        return QUADRANT_SE;
    }

    if (INQUAD(x, y, q, sw)) {
        return QUADRANT_SW;
    }

    fprintf(stderr, "Something is wrong with the quadrant algorithm\n");
    exit(1);
}

quadtree_t* __qt_add(quadtree_t* qt, double x_pos,
    double y_pos, double mass, struct boundary* boundary) {
    /* It is a new quadtree without any actual data */
    if (qt == NULL) {
        qt = qt_add_node(x_pos, y_pos, mass,
            boundary->x_begin, boundary->x_end,
            boundary->y_begin, boundary->y_end);
        return qt;
    }

    /* First, we dynamically calculate the quadrants */
    struct interval_pos quadrants;
    calculate_ints(&quadrants, &(qt->boundary));

    /* Then we figure out which quadrant this belongs to */
    #ifdef DEBUG 
    {
        enum quadrant q = find_quadrant(&quadrants, x_pos, y_pos);

        DPRINTF("Will insert (%.2lf, %.2lf) at quadrant %s of (%.2lf, %.2lf)\n", 
            x_pos, y_pos, quad_desc(q), qt->x_pos, qt->y_pos);
    }
    #endif /* DEBUG */

    switch (find_quadrant(&quadrants, x_pos, y_pos)) {
        case QUADRANT_NE:
            qt->ne = __qt_add(qt->ne, x_pos, y_pos, mass, &(quadrants.ne));
            break;
        case QUADRANT_NW:
            qt->nw = __qt_add(qt->nw, x_pos, y_pos, mass, &(quadrants.nw));
            break;
        case QUADRANT_SE:
            qt->se = __qt_add(qt->se, x_pos, y_pos, mass, &(quadrants.se));
            break;
        case QUADRANT_SW:
            qt->sw = __qt_add(qt->sw, x_pos, y_pos, mass, &(quadrants.sw));
            break;
    }

    return qt;
}

void qt_free(quadtree_t* qt) {
    if (qt == NULL) {
        return ;
    }

    qt_free(qt->ne);
    qt_free(qt->nw);
    qt_free(qt->sw);
    qt_free(qt->se);

    free(qt);
}

#ifdef DEBUG

unsigned long int qt_count(quadtree_t* qt) {
    if (qt == NULL) {
        return 0;
    }

    return 1 + qt_count(qt->ne) + qt_count(qt->se) + qt_count(qt->nw) + qt_count(qt->sw);
}

#endif /* DEBUG */


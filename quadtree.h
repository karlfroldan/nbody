#ifndef QUADTREE_H
#define QUADTREE_H

struct boundary {
    double x_begin;
    double x_end;

    double y_begin;
    double y_end;

    /* (x_begin, y_begin) --------------------- (x_end, y_begin)
     *        |                                      |
     *        |                                      |
     *        |                                      |
     * (x_begin, y_end) ----------------------- (x_end, y_end)
     */
};

struct quadtree {
    struct quadtree* ne;
    struct quadtree* se;
    struct quadtree* nw;
    struct quadtree* sw;

    /* Boundaries */
    struct boundary boundary; 

    /* Positional variables */
    double x_pos;
    double y_pos;

    /* Some properties */
    double mass;

    /* Force */
    double x_force;
    double y_force;

    /* Velocity */
    double v_x;
    double v_y;

    /* Acceleration */ 
    double a_x;
    double a_y;
};

typedef struct quadtree quadtree_t;
/* Add a new celestial body to our quad tree */
quadtree_t* __qt_add(quadtree_t*, double, double, double, struct boundary*);
quadtree_t* qt_add(quadtree_t* qt, double x, double y, double m);

#ifdef DEBUG
unsigned long int qt_count(quadtree_t* qt);
#endif /* DEBUG */

void qt_free(quadtree_t*);

#endif /* QUADTREE_H */

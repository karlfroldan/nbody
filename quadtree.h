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

typedef enum {
    /* An internal node only contains the coordinate of the 
     * center of gravity and the total mass of that node.
     * It should not contain any celestial body. */
    NODE_INTERNAL,
    /* An external node contains the body itself plus its 
     * properties such as position, force, and velocity */
    NODE_EXTERNAL
} nodetype_t;

struct body {

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

struct quadtree {
    struct quadtree* ne;
    struct quadtree* se;
    struct quadtree* nw;
    struct quadtree* sw;

    /* If the node is an internal node, then this is the center-of-mass.
     * If it is an external node, then this is the position of the 
     * celestial body itself */
    double x_pos;
    double y_pos;

    /* If the node is an internal node, then this is the total sum of all 
     * the bodies. If it is an external node, then this is the mass of the 
     * celestial body itself */
    double mass;

    /* Boundaries */
    struct boundary boundary; 

    /* What kind of node is this tree? */
    nodetype_t node_type;

    /* This could actually be null */
    struct body* body;
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

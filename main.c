#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "quadtree.h"

#define BUFSIZE 64

int main() {
    char readbuf[BUFSIZ];
    FILE* f = fopen("data.dat", "r");
    quadtree_t* qt = NULL;

    double x, y, mass;

    if (!f) {
        fprintf(stderr, "file not found\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(readbuf, BUFSIZ, f) != NULL) {
        sscanf(readbuf, "(%lf, %lf) %lf", &x, &y, &mass);
        DPRINTF("(%.3lf, %.3lf) %.3lf\n", x, y, mass);
        DPRINTF("/////////////////////////////////////\n");
        qt = qt_add(qt, x, y, mass);
    }

    fclose(f);

    #ifdef DEBUG 
    unsigned long int count = qt_count(qt);

    DPRINTF("quadtree has %ld elements\n", count);
    DPRINTF("quadtree has %ld bytes or %ld bits\n", 
        sizeof(struct quadtree) * count, 
        sizeof(struct quadtree) * count * 8
    );
    DPRINTF("One node has %ld bytes or %ld bits\n",
        sizeof(struct quadtree),
        sizeof(struct quadtree) * 8
    );
    #endif /* DEBUG */


    qt_free(qt);

    return EXIT_SUCCESS; 
}

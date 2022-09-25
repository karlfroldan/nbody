CFLAGS=-Wall -Wextra -O0 -fPIC -g -DDEBUG
FILES=debug main quadtree
OBJS=$(patsubst %, %.o, $(FILES))

all : $(OBJS)
	cc $(CFLAGS) -o quadtree $^

%.o : %.c
	cc $(CFLAGS) -c -o $@ $^

clean :
	-rm quadtree *.o

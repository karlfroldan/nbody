CFLAGS=-Wall -Wextra -O0 -fPIC -g -DDEBUG
FILES=main quadtree barneshut debug
OBJS=$(patsubst %, %.o, $(FILES))

all : $(OBJS)
	cc $(CFLAGS) -o quadtree $^

%.o : %.c
	cc $(CFLAGS) -c -o $@ $^

clean :
	@rm quadtree
	@rm $(OBJS)

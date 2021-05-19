# NOTE: All indented lines are indented with a tab character.

CFLAGS = -ansi -pedantic -O0 -Wall
DFLAGS = -g

all: mazewalker

mazewalker: mazewalker.c
	gcc mazewalker.c $(CFLAGS) $(DFLAGS) -o mazewalker

test: mazewalker
	./mazewalker sample_maze.txt

clean:
	rm -fv mazewalker *.o 

zip:
	zip ${USER}_a1.zip Makefile mazewalker.c
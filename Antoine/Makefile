CFLAGS = -Wall -Wextra -std=c99

all : solver

solver : solver.o solving.o
	gcc $(CFLAGS) -o solver solver.o solving.o

clean:
	rm -f *.o
	rm -f *.result
	rm -f solver

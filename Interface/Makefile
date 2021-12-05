CC= gcc

CFLAGS= -g -Wall -Wextra -std=c99 -O3 `pkg-config --cflags sdl SDL_image` `pkg-config --cflags gtk+-3.0`
LDLIBS= -pthread -lpthread `pkg-config --libs sdl SDL_image` `pkg-config --libs gtk+-3.0` -rdynamic



user_interface: user_interface.o
	$(CC)	-g -o user_interface user_interface.o $(LDLIBS)

user_interface.o: user_interface.c
	$(CC)	$(CFLAGS)	-c user_interface.c

# remove object files and executable when user executes "make clean"
clean:
	rm user_interface *.o -f


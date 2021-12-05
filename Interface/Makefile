CC= gcc

CFLAGS= -g -Wall -Wextra -std=c99 -O3 `pkg-config --cflags sdl SDL_image` `pkg-config --cflags gtk+-3.0` -MMD
LDLIBS= `pkg-config --libs sdl SDL_image SDL_gfx` `pkg-config --libs gtk+-3.0` -rdynamic


user_interface: user_interface.o grayscale2.o pixel_operations.o
	$(CC)	-g -o user_interface grayscale2.o pixel_operations.o user_interface.o -lm $(LDLIBS)

user_interface.o: user_interface.c
	$(CC)	$(CFLAGS)	-c user_interface.c

solver: solver.o solving.o
	$(CC)	-g -o solver solver.o solving.o -lm $(LDLIBS)
# remove object files and executable when user executes "make clean"
clean:
	rm user_interface solver *.o *.d *.result -f


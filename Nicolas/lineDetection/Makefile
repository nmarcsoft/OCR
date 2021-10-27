# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image SDL_gfx ` -lm

all: display grayscale

display: display.o
grayscale: pixel_operations.o grayscale.o
grayscale2: pixel_operations.o grayscale2.o
grayscale.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale
	${RM} grayscale2
# END

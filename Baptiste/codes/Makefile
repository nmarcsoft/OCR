# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS= -lSDL_gfx -lm
LDLIBS= `pkg-config --libs sdl SDL_image`

all: display grayscale

display: display.o
grayscale: pixel_operations.o grayscale.o  pixel_operations.c grayscale.c

grayscale.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale

# END

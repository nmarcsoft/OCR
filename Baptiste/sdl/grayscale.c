#include <stdio.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <SDL/SDL_rotozoom.h>
#include <math.h>
void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

SDL_Surface* grayscale(SDL_Surface* image_surface)
{
    int height = image_surface->h;
    int width = image_surface->w;
    for(int x = 0; x < height; x++)
    {
	for(int y = 0; y < width;y++)
	{
	       Uint32 pixel = get_pixel(image_surface, x, y);
	       Uint8 r, g, b;
	       SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
	       int mid = 0.3*r + 0.59*g + 0.11*b;
	       Uint32 pixel2 = SDL_MapRGB(image_surface->format, mid, mid, mid);
	       put_pixel(image_surface, x, y, pixel2);
	}
    }
    SDL_SaveBMP(rotozoomSurface(image_surface,0,1,1),"grayscale.bmp");
return image_surface;
}
int main(int argc , char *argv[])
{
    char* image = argv[1];
    SDL_Surface* image_surface;
    init_sdl();
    image_surface = load_image(image);
    grayscale(image_surface);
	return 0;
}

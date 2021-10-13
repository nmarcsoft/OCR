#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

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


int main()
{

    SDL_Surface* image_surface;
    init_sdl();

    image_surface = load_image("Case.jpg");
    // CPT : Count number of white pixels, if > 200 -> black line detected
    int cpt = 0;
    // VARIABLES :
    // cptHeight -> count lines vertical
    // cptWidth -> count lines horizontal
    int cptHeight = 0;
    int cptWidth = 0;
    int width = image_surface->w;
    int height = image_surface->h;
    for (int i = 0; i < width; i++)
    {
	for (int j = 0; j < height; j++)
	{
		Uint32 pixel = get_pixel(image_surface, i, j);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		// Check if the pixel is black
		if (r + g + b < 10)
		{
			// We have a black pixel
			int test = 1;
			while (i < width && test)
			{
			i++;
			Uint32 pixel = get_pixel(image_surface, i, j);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			if ((r + g + b) > 20)
			{
				test = 0;
			}
			else
			{
				cpt += 1;
			}
			}
			if (cpt > 200)
			{
				cptHeight += 1;
				j += height/20;
			}
		}
	}
    }
for (int j = 0; j < height; j++)
    	{
	for (int i = 0; i < width; i++)
	{
		Uint32 pixel = get_pixel(image_surface, i, j);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		if (r + g + b < 10)
		{
			// We have a black pixel
			int test = 1;
			while (j < height && test)
			{
			j++;
			Uint32 pixel = get_pixel(image_surface, i, j);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			if ((r + g + b) > 20)
			{
				test = 0;
			}
			else
			{
				cpt += 1;
			}
			}
			if (cpt > 200)
			{
				cptWidth += 1;
				i += width/200;
			}
		}
	}
    }
    printf("Ligne droite - gauche : %d ligne haut - bas : %d", cptWidth, 
		    cptHeight);
    return 0;
    }


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
    //SDL_Surface* screen_surface;
    init_sdl();

    image_surface = load_image("image_01.jpeg");
    //screen_surface = display_image(image_surface);
    // VARIABLES :
    // cptHeight -> count lines vertical
    // cptWidth -> count lines horizontal
    int cptHeight = 0;
    int cptWidth = 0;
    int width = image_surface->w;
    int height = image_surface->h;
    for (int i = 0; i < height; i++)
    {
	for (int j = 0; j < width; j++)
	{
		Uint32 pixel = get_pixel(image_surface, i, j);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		// Check if the pixel is black
		if (r + g + b < 50)
		{
			
			// We have a black pixel
			// CPT : Count number of white pixels,
			// if > 980 -> black line detected
			int cpt = 0;
			int test = 1;
			while (j < width-1 && test)
			{
			j++;
			Uint32 pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
			if (r + g + b < 50)
			{
				cpt += 1;
			}
			else
			{
				test = 0;
			}
			}
			if (cpt > 980)
			{
				i += 20;
				cptWidth += 1;
			}
		}
	}
    }

    // Now, lets check for the vectical lines
    // It's the same code but with different index and variables
    printf("Ligne Horizontale : %d", cptWidth);
    for (int i = 0; i < width; i++)
    {
	for (int j = 0; j < height; j++)
	{
		Uint32 pixel = get_pixel(image_surface, j, j);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		// Check if the pixel is black
		if (r + g + b < 50)
		{
			
			// We have a black pixel
			// CPT : Count number of white pixels,
			// if > 200 -> black line detected
			int cpt = 0;
			int test = 1;
			while (j < height-1 && test)
			{
			j++;
			Uint32 pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
			if (r + g + b < 50)
			{
				cpt += 1;
			}
			else
			{
				test = 0;
			}
			}
			if (cpt > 980)
			{
				i += 20;
				cptHeight += 1;
			}
		}
	}
    }
    printf("Ligne Verticale : %d", cptHeight);
    return 0;
    }


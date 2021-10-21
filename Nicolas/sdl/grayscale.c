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

int widthCase(SDL_Surface* image_surface, int width, int height)
{
    int toReturn = 0;
    int test;
for (int i = 0; i < height; i++)
    {
	for (int j = 0; j < width; j++)
	{
		Uint32 pixel = get_pixel(image_surface, j, i);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		// Check if the pixel is black
		if (r == 0)
		{
			
			// We have a black pixel
			// CPT : Count number of white pixels,
			// if > 980 -> black line detected
			int cpt = 0;
			test = 1;
			while (j < width-1 && test)
			{
			j++;
			Uint32 pixel = get_pixel(image_surface, j, i);
			SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
			if (r == 0)
			{
			// Create a red pixel
			cpt += 1;
			}
			else
			{
				test = 0;
			}
			}
			// Test if we have many black pixels -> it's a lin
			if (cpt > width-20)
			{
				i += width/90;
				toReturn = cpt/9;
				
				break;
			}
		}
	}
	if (test)
	{
		break;
	}
    }
	return toReturn;
}

int heightCase(SDL_Surface* image_surface, int width, int height)
{
int test = 0;
int caseY = 0;
	for (int i = 0; i < width; i++)
    {
	for (int j = 0; j < height; j++)
	{
		Uint32 pixel = get_pixel(image_surface, i, j);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		// Check if the pixel is black
		if (r == 0)
		{
			
			// We have a black pixel
			// CPT : Count number of white pixels,
			// if > 200 -> black line detected
			int cpt = 0;
			test = 1;
			while (j < height-1 && test)
			{
			j++;
			Uint32 pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
			if (r == 0)
			{
				Uint32 pixel2 = SDL_MapRGB(image_surface->format, 255, 0, 0);
			// Put the pixel in the image
			put_pixel(image_surface, j, i, pixel2);
			cpt += 1;
			}
			else
			{
				test = 0;
			}
			}
			// Test if we have many black pixels -> it's a line
			if (cpt > height-20)
			{
				i += height/90;
				caseY = cpt/9;
				break;
			}
		}
	}
    }
	return caseY;
 
}


// FUNCTION that check if on the 5 next pixel, from top to bot, we
// have the same color of pixel
int getAround(SDL_Surface* image_surface, int x, int y)
{
	Uint8 r, g, b;
	int toReturn = 0;
	Uint32 pixel = get_pixel(image_surface, x, y);
	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	int beEqual = r + g + b;
	int test;
	for (int i = 1; i < 5; i++)
	{
		test = 0;
		for (int j = 1; j < 5; j++)
		{
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			if (r + g + b != beEqual)
			{
				break;
			}
			test+=1;
		}
		if (test != 4)
		{
			break;
		}
		else {
			if (i == 4)
			{
				toReturn = 1;
			}
		}
	}
	return toReturn;
}

int main()
{

    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    init_sdl();

    image_surface = load_image("imagemodif2.bmp");
    screen_surface = display_image(image_surface);
    // VARIABLES :
    // cptHeight -> count lines vertical
    // cptWidth -> count lines horizontal
    int width = image_surface->w;
    int height = image_surface->h;
    int caseX = widthCase(image_surface, width, height);
    int caseY = heightCase(image_surface, width, height);
    printf("Out of loop : %d\n", caseX);
    printf("Out of loop : %d\n", caseY);
    printf("%d\n", getAround(image_surface, 158, 174));
    update_surface(screen_surface, image_surface);
    //wait_for_keypressed();
    return 0;
}


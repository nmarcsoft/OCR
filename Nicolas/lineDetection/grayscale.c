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

int * DetectStart(int * histo, int width, int height, int * coord)
    {
	int cpt = 0;
	int tot = 0;
	int i = 0;
	int j = 0;
	int line = 0;
        while (i < 345/*height-10*/ && !line)
	{
	   cpt = 0;
	   j = 0;
	   while (j < width - 10 && !line)
	   {
		// Check on a square of 10px x 10px
		for (int k = i; k < 5; k++)
		{
		    for (int l = j; l < 5; l++)
		    {
			    //printf("x = %d i -> %d and k -> %d; y = %d j -> %d and l -> %d \n      histo value = %d", (i+k), i, k, (j+l), j, l, *(histo+((i+k) * width + (j+l))));
			    if ((*(histo+(l * width + k))) == 1)
			    {
				    cpt += 1;
				    printf("inscrease\n");
			    }
			    printf("%d\n", cpt);
		    }
		}
		// If their is more than 70 black pixel, add line
		if (cpt >= 10)
		{
			tot += 1;
			if (tot > width/10)
			{
				line = 1;
			}
		}
		j+=10;
	   }
	   i++;
	}
	printf("x = %d; y = %d\n", j, i);
	*coord = j;
	*(coord+1) = i;
	return coord;
    }


int * initializeHisto(int * histo, SDL_Surface* image_surface, int width, int height)
{
	Uint32 pixel;
	Uint8 r, b, g;
	for (int i = 0; i < height; i++)
	{
	 for (int j = 0; j < width; j++)
	 {
	   pixel = get_pixel(image_surface, j, i);
	   SDL_GetRGB(pixel, image_surface->format, &r, &b, &b);
	   if (r > 127)
	   {
	     *(histo+ (i*width +j)) = 1;
	     printf("put 1");
	   }
	   else
	    {
	     *(histo + (i*width + j)) = 0;
	    }
	 }
	}
	return histo;
}

void printMatrix(int* a, int height, int width)
{
	printf("{");
	for (int i = 0; i < height; i++)
	{
	   for (int j = 0; j < width; j++)
	   {
	    int test = *(a + (i*width + j));
	    printf("%d ", test);
	   }
	if (i!=height-1)
	{
	  printf("\n");
	}
	}
   printf("}");
   printf("\n\n");
}

int main()
{
    SDL_Surface* image_surface;
    init_sdl();
    image_surface = load_image("imagemodif2.bmp");
    // VARIABLES :
    int width = image_surface->w;
    int height = image_surface->h;
    int tmp = 0;
    int endLine = 1;
    int size = width * height;
    int *histo = 0;
    histo = (int*) malloc(size * sizeof(int));
    int *start;
    if (histo == NULL)
    {
     printf("Can't allocated memory");
    }
    int * coord = (int*) malloc(2 * sizeof(int));
    histo = initializeHisto(histo, image_surface, width, height);
    printMatrix(histo, height, width);
    DetectStart(histo, width, height, coord);
    printf("x = %d, y = %d", *start, *(start + 1));
    return 0;
}


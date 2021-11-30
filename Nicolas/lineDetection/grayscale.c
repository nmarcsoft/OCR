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

void copySurface(int * from, SDL_Surface* to, int width, int height)
{
	Uint32 pixel;
	for (int i = 0; i < height; i++)
	{
	    for (int j = 0; j < width; j++)
	    {
	    	if (*(from + (i*width+j)) == 1)
		{
		  pixel = SDL_MapRGB(to->format, 1, 1, 1);
		  put_pixel(to, j, i, pixel);
		}
		else
		{
		  pixel = SDL_MapRGB(to->format, 1, 1, 1);
		  put_pixel(to, j, i, pixel);
		}
	    }
	}
	SDL_SaveBMP(to, "out.bmp");
	printf("Saved");
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
	int ReturnX = 0;
	int ReturnY = 0;
        while (i < height-10 && !line)
	{
	   ReturnY = i;
	   j = 0;
	   ReturnX = j;
	   tot = 0;
	   while (j < width - 10 && !line)
	   {
	   cpt = 0;
		// Check on a square of 10px x 10px
		for (int k = i; k < i + 5; k++)
		{
		    for (int l = j; l < j + 5; l++)
		    {
			    if ((*(histo+(k * width + l))) == 0)
			    {
			    	    
				    cpt += 1;
			    }
			    
		    }
		}
		// If their is more than 70 black pixel, add line
		if (cpt >= 20)
		{
			tot += 1;
			if (tot >= 50)
			{
				line = 1;
			}
			cpt = 0;
		}
		j+=10;
	  }
	   i+=10;
	}
	*coord = ReturnX;
	*(coord+1) = ReturnY;
	return coord;
    }

int * getCase(int * histo, int width, int height, int * coord, int size, int num, int * Case)
{
	if (*coord == 0)
	{
		*coord = 1;
	}
	printf("%d", *(histo+1300));
	// Merci les noeuds au cerveau !!!
	//int j = *(histo+(*(coord+1) + num * height/9));
	//int i = *(histo+(*coord) + num * width/9);
	int toGoH = (height-1)/9;
	int toGoW = (width-1)/9;
	int toPutX = 0;
	int c = 0;
	for (int i = 0; i < toGoH; i++)
	{
	    for (int j = 0; j < toGoW; j++)
	    {
//	printf("coord j = %d ", (j + *coord));
		if (*(histo + ((i * width + j))) != 0 && *(histo + ((i * width + j))) != 1)
		{
			printf("DICK");
		}
	//printf("limitTest (10000) = %d\n",/*((i + *(coord+1) * toGoW) + j + *(coord)));*/*(histo+((i * width + j))));
		printf("x = %d : y = %d\n", i + *(coord+1), j + *coord);
		printf("            value = %d\n", *(histo + i + *(coord+1) * width + j + *coord));
	    	*(Case+(i * toGoW + j)) = *(histo + (((i + *(coord+1)) * width + j + *coord))); 
	    }
	    toPutX++;
	}
	printf("toGow = %d, toGoH = %d", toGoW, toGoH);
	return Case;
	/*
	int get = 0;
	while (i < height && !get)
	{
		while (
*/
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
	printf("{\n");
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
    image_surface = load_image("imagemodif3.bmp");
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
    //printMatrix(histo, height, width);
    coord = DetectStart(histo, width, height, coord);
    printf("Coord de départ : x -> %d, y -> %d\n", *coord, *(coord+1));
    int * Case = 0; 
    Case = (int*) malloc(size/81 * sizeof(int));
    if (Case == NULL)
    {
	printf("lineDetection : Can't allocated memory");
    }
    Case = getCase(histo, width, height, coord, size, 2, Case);
    //printf("allocated : %d, loop : %d", size/81, ((width-1)/9)*((height-1)/9));
    copySurface(Case, load_image("out.bmp"), 99, 99);
    printMatrix(Case, 111, 111);
    return 0;
}


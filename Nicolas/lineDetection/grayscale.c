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
		  pixel = SDL_MapRGB(to->format, 255, 255, 255);
		  put_pixel(to, j, i, pixel);
		}
		else
		{
		  pixel = SDL_MapRGB(to->format, 0, 0, 0);
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
	// Merci les noeuds au cerveau !!!
	//int j = *(histo+(*(coord+1) + num * height/9));
	//int i = *(histo+(*coord) + num * width/9);
	int toGoH = (height-1)/9;
	int toGoW = (width-1)/9;
	int toGet = num * toGoW;
	printf("%d", toGet);
	int cpt = 0;
	for (int i = 0; i < toGoH; i++)
	{
	    for (int j = 0; j < toGoW; j++)
	    {
		if (*(histo + ((i * width + j))) != 0 && *(histo + ((i * width + j))) != 1)
		{
			printf("DICK");
		}
	    	*(Case+(i * toGoW + j)) = *(histo + (toGet + ((i + *(coord+1)) * width + j + *coord))); 
		printf("VALUE = %d\n",*(Case+(i * toGoW + j))); 
		cpt++;
	    }
	}
	printf("%d", cpt);
	return Case;
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

int Width(int * histo, int coordX, int width)
{
	int i = coordX;
	while (*(histo + i) == 0 && i < width)
	{
		i++;
	}
	return i;
}

int Height(int * histo, int coordY, int width)
{
	int i = coordY;
	while (*(histo+(i*width)) == 0 && i < width)
	{
		i++;
	}
	return i;
}

int * cut(int * histo, int i, int j, int widthR, int heightR, int width, int * a)
{
	for (int k = 0; k < heightR/9; k++)
	{
		for (int l = 0; l < widthR/9; l++)
		{
*(a + (k * (widthR/9) + l)) = *(histo + ((i + k) * width + (j + l)));
		}
	}
	printf("CALL");
	printf("Done");
	return a;
}

int * Rogne(int * toPrint, int x, int y, int width, int height)
{
	printf("In, x = %d ; y = %d ; maxX = %d ; maxY = %d\n", x, y, width, height);
	int j = 0;
	int getted = 0;
	while (j < width)
	{
		if (*(toPrint + (j)) == 0)
		{
			for (int i = 0; i < height; i++){
				*(toPrint+(i * width + j)) = 1;
			}
		}
		j++;
	}
	int i = 0;
	while (i < height)
	{
		if (*(toPrint + (i * width)) == 0)
		{
		for (int j = 0; j < width; j++)
		{
			*(toPrint+((i*width)+j)) = 1;
		}
		}
		i++;
	}
	printf("\n%d", width);
	return toPrint;

}

void DoneAll(int * histo, int * coord, int width, int height)
{
	int widthReal = Width(histo, *coord, width);
	widthReal = 1000;
	printf("RealW = %d\n", widthReal);
	int heightReal = Height(histo, *coord+1, width);
	heightReal = 1000;
	int *toPrint = (int*) malloc((widthReal/9*heightReal/9) * sizeof(int)); 
	printf("WeightR = %d\n", heightReal);
	int Stop = 0;
	for (int i = *(coord+1); i < heightReal; i++)
	{
		for (int j = *coord; j < widthReal; j++)
		{
			if (*(histo + (i * widthReal + j)) == 1)
			{
	toPrint = cut(histo, i, j, widthReal, heightReal, width, toPrint);
	printf("x = %d ; y = %d", j, i);
	toPrint = Rogne(toPrint, j, i, widthReal/9, heightReal/9);
	copySurface(toPrint, load_image("out.bmp"), widthReal/9, heightReal/9);
			Stop++;
			j+=widthReal/9;
			}
			if (Stop == 9)
			{
			printMatrix(toPrint, widthReal/9, heightReal/9);
			break;
			}
		}
		if (Stop == 9)
		{
		break;
		}
	}
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
   // printMatrix(histo, height, width);
    coord = DetectStart(histo, width, height, coord);
    printf("Coord de départ : x -> %d, y -> %d\n", *coord, *(coord+1));
    //int * Case = 0; 
    //Case = (int*) malloc(size/81 * sizeof(int));
    //if (Case == NULL)
    //{
//	printf("lineDetection : Can't allocated memory");
  //  }
    //Case = getCase(histo, width, height, coord, size, 2, Case);
    //printf("allocated : %d, loop : %d", size/81, ((width-1)/9)*((height-1)/9));
    DoneAll(histo, coord, width, height);
    //copySurface(Case, load_image("out.bmp"), 99, 99);
    //printMatrix(Case, 111, 111);
    return 0;
} 

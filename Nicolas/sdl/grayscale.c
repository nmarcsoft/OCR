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
			// Put the pixel in the image
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
//
// To check black -----> value = 0
// To check white -----> value = 765
int getAround(SDL_Surface* image_surface, int x, int y, int value)
{
	Uint8 r, g, b;
	int toReturn = 0;
	Uint32 pixel = get_pixel(image_surface, x, y);
	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	int test;
	for (int i = 1; i < 20; i++)
	{
		test = 0;
		for (int j = 1; j < 90; j++)
		{
			Uint32 pixel = get_pixel(image_surface, x+j, y+i);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			if (r + g + b != value)
			{
				break;
			}
			test+=1;
		}
		if (test != 89)
		{
			break;
		}
		else {
			if (i == 19)
			{
				toReturn = 1;
			}
		}
	}
	return toReturn;
}

// Put caseX*caseY pixel from (x, y) in "from" -> to "to"
void copySurface(SDL_Surface* from, SDL_Surface* to, int x, int y,
		int caseX, int caseY)
{
	for (int j = 0; j < caseY; j++)
	{
	   for (int k = 0; k < caseX; k++)
	   {
		Uint32 pixel = get_pixel(from, x+k, y+j);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, from->format,&r, &g, &b);
	   	Uint32 pixel2 = SDL_MapRGB(to->format, r, g, b);
		put_pixel(to, k, j, pixel2);
	   }
	}
	printf("image printed with x = %d, y = %d\n", x, y);
	SDL_SaveBMP(to, "out.bmp");
}


int doneAll()
{
    SDL_Surface* image_surface;
    //SDL_Surface* screen_surface;
    image_surface = load_image("imagemodif2.bmp");
    int width = image_surface->w;
    //screen_surface = display_image(image_surface);
    int height = image_surface->h;
    //update_surface(screen_surface, image_surface);
    int array[height][width];
    Uint8 r, g, b;
    Uint32 pixel;
    printf("Go for the first one\n");
    for (int y = 0; y < height; y++)
	{
	for (int x = 0; x < width; x++)
	{
		pixel = get_pixel(image_surface, x, y);
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		if (r > 127)
		{
			// We have a white pixel
			array[y][x] = 1;
		}
		else
		{
			// We have a black pixel
			array[y][x] = 0;
		}
	}
	}
	printf("First Done\n");

    ///////////////////////////
    /// Find longest Horizontal line
    // longestH -> count the max of matrix
    // tmp      -> count max on a line
    // indexH   -> line of the max line
    int indexH[10];
    int tmp;
    int added = 0;
    for (int i = 0; i < height; i++)
    {
	tmp = 0;
	for (int j = 0; j < width; j++)
	{
		if (array[i][j] == 0)
		{
		    // We have a black pixel
		    tmp += 1;
		}
	}
	if (tmp > height / 2)
	{
	     indexH[added] = i;
	     added++;
	     i += height * 0.015;
	}
    }
    //////////////////////////////
    /// Same for Vertical
    int indexV[10];
    added = 0; 
    for (int i = 0; i < width; i++)
    {
	tmp = 0;
	for (int j = 0; j < height; j++)
	{
	    if (array[j][i] == 0)
	    {
		tmp++;
	    }
	}
	if (tmp > width / 2)
	{
	    indexV[added] = i;
	    added++;
	    i += width * 0.015; 
	}
    }

    // We have the coordonate of the long lines,
    // now lets try to get the case
    int toPrint[indexV[1]][indexH[1]];
    tmp = 0;
    int gap = 0;
    int gapX = 0;
    int gapY = 0;
    printf("indexV[0] = %d", indexV[0]);
    printf("indexH %d\n", indexH[0]);
    for (int y = gapY; y < height; y++)
    {
	for (int x = gapX; x < width; x++)
	{
	   if (array[x][y] && gap<= 8)
	  {
	  	//printf("x = %d y = %d gap = %d\n", x, y, gap);
		int toPrint[indexV[gap + 1]][indexH[gap + 1]];
		for (int i = 0; i < indexV[gap+1] - indexV[gap] -1; i++)
		{
			for (int j = 0; j < indexH[gap+1] - indexH[gap] - 2; j++)
			{	
		printf("x = %d y = %d, array = %d\n", j, i, array[x + j][y + i]);
		toPrint[i][j] = array[y + i][x + j];
			}
		}
		tmp++;
		gap++;
		gapX = indexH[gap];
		gapY = indexV[gap];
		/*if (added < 9)
		{
	
		printf("implementation");
		y = indexV[added];
		x = indexH[added];
		}*/
	   }
	   if (tmp == 3)
	   {
		break;
	   }
	}
	if (tmp == 3)
	{
		break;
	}
    }
    printf("test");;
////////////////////////////////////////
///////////////////////////////////////
//         PRINT MATRIX            ///
//    //   ///////////        //  ///
/////////////////////////////////////
/////////////////////////////////////
//                                 //
////          Index qui depasse sur code superieur                  ////
///////                     ///////
//////////////////////////////////
	/*printf("added = %d\nindexV[added + 1] = %d and indexH[added + 1] = %d", added, indexV[added + 1], indexH[added + 1]);
    for (int i = 0; i < indexH[1] - lineStartX; i++)
	{
	for (int j = 0; j < indexV[1] - lineStartX; j++)
	{
		if (toPrint[i][j]){
		printf(" x = %d, y = %d -> %d\n",lineStartX + j, lineStartX + i, toPrint[i][j]);}
	}
	}*/
	return 0;
	}

int main()
{
    init_sdl();
    doneAll();
   //SDL_Surface* case1;

    //case1 = load_image("index.jpg");
    // VARIABLES :
    // cptHeight -> count lines vertical
    // cptWidth -> count lines horizontal

    /*
    int caseX = widthCase(image_surface, width, height)-20;
    int caseY = heightCase(image_surface, width, height)-20  ;
    int tmp = 0;
    int endLine = 1;
    for (int y = 0; y < height; y+=2)
    {
	for (int x = 0; x < width; x+=2)
	{
	    if(getAround(image_surface, x, y, 765))
	    {
		copySurface(image_surface, case1, x, y, caseX, caseY);
		x += caseX;
		tmp += 1;
		endLine += 1;
	    	printf("endLine = %d, tmp = %d", endLine, tmp);
	    }
	    if (endLine%10 ==  0)
	    {
		endLine = 1;
		y += caseY*1.02;
		break;
	    }
	}
 }
 */
    //wait_for_keypressed();
    return 0;
}


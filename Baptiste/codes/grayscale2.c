#include <stdio.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "segmentation.h"
#include "pixel_operations.h"
#include <SDL/SDL_rotozoom.h>
#include <math.h>

#define TEMPS       30 

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

    screen = SDL_SetVideoMode(img->w, img->h, 32, SDL_HWSURFACE);
//    screen = SDL_SetVideoMode(1000, 1000, 32, SDL_SWSURFACE|SDL_ANYFORMAT);
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

void copySurface(int * from, SDL_Surface* to, int width, int height)
{
        printf(" copy  ");
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
        SDL_SaveBMP(to, "new.bmp");
	printf("copy \n");
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
        while (i < height-10 && line == 0)
        {
           ReturnY = i;
           j = 0;
           tot = 0;
           while (j < width - 10 && line == 0)
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
                if (cpt >= 10)
                {
                        tot += 1;
                        if (tot >= 50)
                        {
                                line = 1;
                                ReturnX = j - 50*5;
                                break;
                        }
                }
                j+=5;
                  }
           i+=5;
        }
        *coord = ReturnX+5;
        *(coord+1) = ReturnY;
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
           SDL_GetRGB(pixel, image_surface->format, &r, &b, &g);
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
        //printf("%d ", *(histo + (943 * width + 500)));
        return histo;
}



int Width(int * histo, int * coord, int width)
{
        int x = *coord;
        int y = *(coord+1) + 5;
        int toReturn = 0;
        while (x < width-5)
        {
        toReturn = 0;
        for (int i = 0; i < 5; i++)
        {
        if (*(histo + ((y * width) + (i+x))) == 0)
        {
                toReturn++;
        }
        }
        if (toReturn >= 4)
        {
                x+=5;
        }
        else
        {
            break;
        }
        }
        //printf("debug width x = %d ; y = %d, value = %d, width = %d", x, y, *(histo + (x * width + y)), width);
        while ((*(histo + (x * width + y)) == 0) && x < width)
        {
        x++;}
        return (x - (*coord));
}

int Height(int * histo, int * coord, int height)
{
        int x = *coord;
        int y = *(coord+1);
        int toReturn = 0;
        while (y < height-5)
        {
                toReturn = 0;
                for (int i = 0; i < 5; i++)
                {
                if (*(histo + (((y+i) * height + x))) == 0)
                {
                        toReturn++;
                }
                }
                if (toReturn >= 2)
                {
                        //printf("\nx = %d, y = %d, value = %d\n", x, y, (*(histo + (((y) * height + x)))));
                        y+=5;
                }
                else
                {
                        break;
                }
        }
        return (y - (*(coord+1)));

}


int * cut(int * histo, int i, int j, int widthR, int heightR, int width, int * a)
{
//      printf("CALL");
        for (int k = 0; k < heightR/9; k++)
        {
                for (int l = 0; l < widthR/9; l++)
                {
*(a + (k * (widthR/9) + l)) = *(histo + ((i + k) * width + (j + l)));
                }
        }
//      printf("Done");
        return a;
}


int Rogne(int * toPrint, int width, int height)
{ for (int y = 0; y < height; y++)
        {
                for (int x = 0; x < 10; x++)
                {
                *(toPrint + (y * width + x)) = 1;
                }
        }
        for (int y = 0; y < height; y++)
        {
                for (int x = 0; x < 10; x++)
                {
                *(toPrint + (x * width + y)) = 1;
                }
        }
        for (int y = 0; y < height; y++)
        {
                for (int x = 60; x < width; x++)
                {
                *(toPrint + (y * width + x)) = 1;
                }
        }
        for (int y = 80; y < height; y++)
        {
                for (int x = 0; x < width; x++)
                {
                *(toPrint + (y * width + x)) = 1;
                }
        }
        return height;
}

void DoneAll(int * histo, int * coord, int width, int height)
{
        int y = 0;
        int widthReal = Width(histo, coord, width);
        int heightReal = Height(histo, coord, height);
        int *toPrint = (int*) malloc((widthReal/9*heightReal/9) * sizeof(int));
        int Stop = 0;
	printf("%d",  heightReal);
        for (int i = *(coord+1); i < heightReal; i++)
        {
                for (int j = *coord; j < widthReal; j++)
                {
			
                        if (*(histo + (i * widthReal + j)) == 1)
                        {
        //printf("BEFORE CUT : j = %d; j = %d\n", i , j);
        toPrint = cut(histo, i, j, widthReal, heightReal, width, toPrint);
        /*toPrint = */y = Rogne(toPrint, widthReal/9, heightReal/9);
        //printMatrix(toPrint, 88, 88);
        
printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
copySurface(toPrint, load_image("new.bmp"), widthReal/9, heightReal/9);
                        Stop++;
                        j+=widthReal/9;
                        }
                        if (Stop % 9 == 0)
                        {
                        i+=y-1;
                        //printf("\n%d should be < thant %d", i, heightReal);
                        if (Stop == 72)
                        {
                                i = *(coord+1) + (heightReal - heightReal/9) -1;
                        }
                        }
                        }
        }
}






Uint8 f(Uint8 c, double n)
{
    if(c <= 255 / 2)
        return (Uint8)( (255/2) * pow(((double) 2 * c / 255),n));
    else
        return 255 - f(255 - c, n);
}

int lignedroite (SDL_Surface* image_surface,int i,int j,int toreturn)
{
	if(toreturn>80)
	{
		return 1;
	}
	else
	{
		Uint32 pixel1 = get_pixel(image_surface,i,j-1);
	        Uint8 r1, g1, b1;
	        SDL_GetRGB(pixel1, image_surface->format,
		                                       &r1, &g1, &b1);
		if(r1 == 0)
		{
			return lignedroite(image_surface,i,j-1,toreturn+1);
		}
		else
		{
			return 0;
		}

	}
}
//Straight line


int lignehorizon (SDL_Surface* image_surface,int i,int j,int toreturn)
{
	if(toreturn>80)
	{
		return 1;
	}
	else
	{
		Uint32 pixel1 = get_pixel(image_surface,i+1,j);
	        Uint8 r1, g1, b1;
	        SDL_GetRGB(pixel1, image_surface->format,
		                                       &r1, &g1, &b1);
		if(r1 == 0)
		{
			return lignehorizon(image_surface,i+1,j,toreturn+1);
		}
		else
		{
			return 0;
		}

	}
}


int line (SDL_Surface* image_surface,int i,int j,int toreturn)
{
	if(toreturn>40)
	{
		return 1;
	}
	else
	{
		Uint32 pixel1 = get_pixel(image_surface,i,j-1);
	        Uint8 r1, g1, b1;
	        SDL_GetRGB(pixel1, image_surface->format,
		                                       &r1, &g1, &b1);
		if(r1 == 0)
		{
			return line(image_surface,i,j-1,toreturn+1);
		}

		Uint32 pixel2 = get_pixel(image_surface,i-1,j-1);
	        Uint8 r2, g2, b2;
	        SDL_GetRGB(pixel2, image_surface->format,
		                                       &r2, &g2, &b2);
		if(r2 == 0)
		{
			return line(image_surface,i-1,j-1,toreturn+1);
		}	

		Uint32 pixel3 = get_pixel(image_surface,i+1,j-1);
	        Uint8 r3, g3, b3;
	        SDL_GetRGB(pixel3, image_surface->format,
		                                       &r3, &g3, &b3);
		if(r3 == 0)
		{
			return line(image_surface,i+1,j-1,toreturn+1);
		}

		else
		{
			return 0;
		}

	}
}
//Line


    
SDL_Surface* pretraitement(SDL_Surface* image_surface)                                                                     
{                                                                              
    
    int width = image_surface->w;                                              
    int height = image_surface->h;                                             
    float zoom = 0;                                                            
    float w = image_surface->w;                                                
    float h = image_surface->h;                                                
    float mid = 0;
    int min_gray =0;         
                                                       
                                                                           
    if(height > width)                                                         
    {                                                                          
	zoom = 1000/h;                                                         
    }                                                                          
    else                                                                       
    {                                                                          
	zoom = 1000/w;                                                        
    }   
//1000x1000 window                                            
SDL_SaveBMP(rotozoomSurface(image_surface,0,zoom,1),"image3.bmp");
image_surface = load_image("image3.bmp");

      width = image_surface->w;
      height = image_surface->h;
     w = image_surface->w;
     h = image_surface->h;

                                                  
 
    for(int x = 0; x < width; x++)
    {
       for(int y = 0; y < height;y++)
       {
         Uint32 pixel = get_pixel(image_surface, x, y);
         Uint8 r, g, b;
         SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
         mid = 0.3*r + 0.59*g + 0.11*b;
	 if (mid<min_gray)
	 {
		 min_gray = mid;
	 }
         Uint32 pixel2 = SDL_MapRGB(image_surface->format, mid, mid, mid);
         put_pixel(image_surface, x, y, pixel2);
       }
    }
//grayscale

for(int x = 1; x < width-1; x++)
    {
        for(int y = 1; y < height-1;y++)
        {
Uint32 pixel = get_pixel(image_surface,x,y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format,
                                       &r, &g, &b);
	int tableau[9];
Uint32 pixel1 = get_pixel(image_surface,x-1,y-1);
            Uint8 r1, g1, b1;
            SDL_GetRGB(pixel1, image_surface->format,
                                       &r1, &g1, &b1);
Uint32 pixel2 = get_pixel(image_surface,x-1,y);
            Uint8 r2, g2, b2;
            SDL_GetRGB(pixel2, image_surface->format,
                                       &r2, &g2, &b2);
Uint32 pixel3 = get_pixel(image_surface,x-1,y+1);
            Uint8 r3, g3, b3;
            SDL_GetRGB(pixel3, image_surface->format,
                                       &r3, &g3, &b3);
Uint32 pixel4 = get_pixel(image_surface,x,y-1);
            Uint8 r4, g4, b4;
            SDL_GetRGB(pixel4, image_surface->format,
                                       &r4, &g4, &b4);
Uint32 pixel5 = get_pixel(image_surface,x,y+1);
            Uint8 r5, g5, b5;
            SDL_GetRGB(pixel5, image_surface->format,
                                       &r5, &g5, &b5);
Uint32 pixel6 = get_pixel(image_surface,x+1,y-1);
            Uint8 r6, g6, b6;
            SDL_GetRGB(pixel6, image_surface->format,
                                       &r6, &g6, &b6);
Uint32 pixel7 = get_pixel(image_surface,x+1,y);
            Uint8 r7, g7, b7;
            SDL_GetRGB(pixel7, image_surface->format,
                                       &r7, &g7, &b7);
Uint32 pixel8 = get_pixel(image_surface,x+1,y+1);
            Uint8 r8, g8, b8;
            SDL_GetRGB(pixel8, image_surface->format,
                                       &r8, &g8, &b8);
		tableau[0] = r1;
		tableau[1] = r2;
		tableau[2] = r3;
		tableau[3] = r4;
		tableau[4] = r5;
		tableau[5] = r6;
		tableau[6] = r7;
		tableau[7] = r8;
		tableau[8] = r;
		for (int t = 0; t < 8; t++)
		{
			for (int r = 0; r < 8; r++)
                	{
				if(tableau[r]>tableau[r+1])
				{
					int temp = tableau[r];
					tableau[r] = tableau[r+1];
					tableau[r+1] = temp;
				}
                	}
		}
		Uint32 pixeltmp = SDL_MapRGB
                (image_surface->format, tableau[4], tableau[4], tableau[4]);
                put_pixel(image_surface, x, y, pixeltmp);
		}
}
//Median Filter
                                                  
//Contrast

    float k = 0.5;
    // calculation R = o max

    float T = 0;
    int tottemp = 0;
    int moytemp = 0;
    float var = 0;
    float maxvar = 0;
    int bordw = 0;


for (int tempw = 0; tempw < width; tempw+=25)
{
	if(tempw>width)
	{	bordw = width%25;
		tempw -= bordw;}
	for(int temph = 0; temph<=height-25; temph+=25)
	{
		for(int x = tempw ;x<tempw+25;x++)
		{
			for(int y = temph; y<temph+25;y++)
			{
				Uint32 pixel = get_pixel(image_surface, x, y);
        	        	Uint8 r, g, b;
	                	SDL_GetRGB(pixel, image_surface->format, 
						&r, &g, &b);
        		        tottemp += r;
				var += (r-moytemp)*(r-moytemp);
        			
			}
		}
		moytemp = tottemp / 625;	
		var = sqrtf(var/625);
		if(var>maxvar)
		{
			maxvar = var;
		}
		T = (1-k)*moytemp + k*min_gray + k* (var/(maxvar*
					(moytemp-min_gray)));
		for(int x = tempw ;x<tempw+25;x++)
                {
                        for(int y = temph; y<temph+25;y++)
                        {
                                Uint32 pixel = get_pixel(image_surface, x, y);
                                Uint8 r, g, b;
                                SDL_GetRGB(pixel, image_surface->format, 
						&r, &g, &b);
                     		if (r < T)
                		{
					Uint32 pixel2 = SDL_MapRGB
					(image_surface->format, 0, 0, 0);
  					put_pixel(image_surface, x, y, pixel2);
				}
        		        else
               			{
					Uint32 pixel2 = SDL_MapRGB
					(image_surface->format, 255, 255, 255);
					put_pixel(image_surface, x, y, pixel2);
                		}
			}
		}
		var = 0;
		tottemp = 0;
	}
	var = 0;
	tottemp = 0;
}
//Binarisation
                                                  
 
	for(int k = 0; k <= width-1; k+=width)
    	{
   		for(int l = 0; l <= height-1; l++)
               	{
                	Uint32 pixel2 = SDL_MapRGB
                        (image_surface->format, 255, 255, 255);
                        put_pixel(image_surface, k, l, pixel2);
                }
        }
	for(int k = 0; k <= height-1; k+=height)
    	{
   		for(int l = 0; l <= width-1; l++)
               	{
                	Uint32 pixel2 = SDL_MapRGB
                        (image_surface->format, 255, 255, 255);
                        put_pixel(image_surface, l, k, pixel2);
                }
        }
//white line around the window

    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetCaption("Type 'a' to save, anything to rotate", NULL);

	int x = 0;
	int y = height/2;
	int white = 0;
	int black = 0;
	Uint32 pixel = get_pixel(image_surface, x, y);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
	
	Uint32 pixelym= get_pixel(image_surface, x, y-30);
        Uint8 rm, gm, bm;
        SDL_GetRGB(pixelym, image_surface -> format, &rm, &gm, &bm);
        
	Uint32 pixelyp = get_pixel(image_surface, x, y+30);
        Uint8 rp, gp, bp;
        SDL_GetRGB(pixelyp, image_surface -> format, &rp, &gp, &bp);
	SDL_SaveBMP(rotozoomSurface(image_surface,0,1,1),"image3.bmp");
	image_surface = load_image("image3.bmp");                         
                                                                               
	width = image_surface->w;                                              
	height = image_surface->h;                       
	for(int cpt =0;cpt<3;cpt++)
	{
	for(int x = 1; x < width-1; x++)
	    {
	        for(int y = 1; y < height-1;y++)
	        {
	Uint32 pixel = get_pixel(image_surface,x,y);
	            Uint8 r, g, b;
	            SDL_GetRGB(pixel, image_surface->format,
	                                       &r, &g, &b);
		if(r==0){
			int tableau[8];
	Uint32 pixel1 = get_pixel(image_surface,x-1,y-1);
	            Uint8 r1, g1, b1;
	            SDL_GetRGB(pixel1, image_surface->format,
	                                       &r1, &g1, &b1);
	Uint32 pixel2 = get_pixel(image_surface,x-1,y);
	            Uint8 r2, g2, b2;
	            SDL_GetRGB(pixel2, image_surface->format,
	                                       &r2, &g2, &b2);
	Uint32 pixel3 = get_pixel(image_surface,x-1,y+1);
	            Uint8 r3, g3, b3;
	            SDL_GetRGB(pixel3, image_surface->format,
	                                       &r3, &g3, &b3);
	Uint32 pixel4 = get_pixel(image_surface,x,y-1);
	            Uint8 r4, g4, b4;
	            SDL_GetRGB(pixel4, image_surface->format,
	                                       &r4, &g4, &b4);
	Uint32 pixel5 = get_pixel(image_surface,x,y+1);
	            Uint8 r5, g5, b5;
	            SDL_GetRGB(pixel5, image_surface->format,
	                                       &r5, &g5, &b5);
	Uint32 pixel6 = get_pixel(image_surface,x+1,y-1);
	            Uint8 r6, g6, b6;
	            SDL_GetRGB(pixel6, image_surface->format,
	                                       &r6, &g6, &b6);
	Uint32 pixel7 = get_pixel(image_surface,x+1,y);
	            Uint8 r7, g7, b7;
	            SDL_GetRGB(pixel7, image_surface->format,
	                                       &r7, &g7, &b7);
	Uint32 pixel8 = get_pixel(image_surface,x+1,y+1);
	            Uint8 r8, g8, b8;
	            SDL_GetRGB(pixel8, image_surface->format,
	                                       &r8, &g8, &b8);
			tableau[0] = r1;
			tableau[1] = r2;
			tableau[2] = r3;
			tableau[3] = r4;
			tableau[4] = r5;
			tableau[5] = r6;
			tableau[6] = r7;
			tableau[7] = r8;
	
			for (int t = 0; t < 8; t++)
			{
				for (int r = 0; r < 7; r++)
	                	{
					if(tableau[r]>tableau[r+1])
					{
						int temp = tableau[r];
						tableau[r] = tableau[r+1];
						tableau[r+1] = temp;
					}
	                	}
			}
			Uint32 pixeltmp = SDL_MapRGB
	                (image_surface->format, tableau[2], tableau[2], tableau[2]);
	                put_pixel(image_surface, x, y, pixeltmp);
			}
}}}
                                                  
//Noise suppression

	SDL_SaveBMP(rotozoomSurface(image_surface,0,1,1),"image3.bmp");
        image_surface = load_image("image3.bmp");
	y = height/2;	
	while (white==0)
	{
		Uint32 pixel = get_pixel(image_surface, x, y);
		SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
		x+=1;
		if(r == 255){white = 1;}	
	}
	// Waiting for one white pixel
	
	while (black==0 && !(line(image_surface,x,y,0)))
	{
		x+=1;
		Uint32 pixel = get_pixel(image_surface, x, y);
		SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
		if(r == 0){black = 1;}	
	}

	//Waiting for one black pixel
	double angleline = 0;
	double linea = 0;
	int debug = 0;
	while(!(lignedroite(image_surface,x,y,0)))
         {
		debug = 1;	
		angleline += 1;
		y = height/2;
		SDL_SaveBMP(rotozoomSurface(image_surface,1,1,1),"image3.bmp");
		image_surface = load_image("image3.bmp");
		width = image_surface->w;                                      
		height = image_surface->h;                       
		Uint32 pixel = get_pixel(image_surface, x, y);
                SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
		if(r != 0)
		{
			x = 0;
			white =0;
			black = 0;
 			Uint32 pixel = get_pixel(image_surface, x, y);
                        SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
			while (white==0)
		        {
                 		Uint32 pixel = get_pixel(image_surface, x, y);
                 		SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
                 		x+=1;
                 		if(r == 255){white = 1;}
      			}
			while (black==0 && !(line(image_surface,x,y,0)))
			{
				x+=1;
				Uint32 pixel = get_pixel(image_surface, x, y);
				SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
				if(r == 0){black = 1;}	
			}
			while(r != 0)
			{	
				x+=1;
				Uint32 pixel = get_pixel(image_surface, x, y);
   	        		SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
			}//adjust if the rotation loose the line
		}

		
	}	
	angleline = angleline*0.0174533;
	linea = tan(angleline)*width;
	for(int k = 0; k < width; k++)
    	{
   		for(int l = 0; l < linea; l++)
               	{
                	Uint32 pixel2 = SDL_MapRGB
                        (image_surface->format, 255, 255, 255);
                        put_pixel(image_surface, k, l, pixel2);
                }
        }
	for(int k = 0; k < width; k++)
    	{
   		for(int l = height-linea; l < height; l++)
               	{
                	Uint32 pixel2 = SDL_MapRGB
                        (image_surface->format, 255, 255, 255);
                        put_pixel(image_surface, k, l, pixel2);
                }
        }
	for(int k = 0; k < height; k++)
    	{
   		for(int l = 0; l < linea; l++)
               	{
                	Uint32 pixel2 = SDL_MapRGB
                        (image_surface->format, 255, 255, 255);
                        put_pixel(image_surface, l, k, pixel2);
                }
        }
	for(int k = 0; k < height; k++)
    	{
   		for(int l = width-linea; l < width; l++)
               	{
                	Uint32 pixel2 = SDL_MapRGB
                        (image_surface->format, 255, 255, 255);
                        put_pixel(image_surface, l, k, pixel2);
                }
        }
	//Black rotation -> white
	
	//rotate while no line found
if(debug == 0){
SDL_SaveBMP(rotozoomSurface(image_surface,0,1,1),"image3.bmp");}
else
{
printf("??");
 SDL_SaveBMP(rotozoomSurface(image_surface,-1,1,1),"image3.bmp");
 image_surface = load_image("image3.bmp");
width = image_surface->w;
height = image_surface->h;
	for(int k = 0; k < 20; k++)
    	{
   		for(int l = 0; l < height; l++)
               	{
                	Uint32 pixel2 = SDL_MapRGB
                        (image_surface->format, 255, 255, 255);
                        put_pixel(image_surface, k, l, pixel2);
                }
        }//gauche
	for(int k = width-20; k <width; k++)
    	{
   		for(int l = 0; l < height; l++)
               	{
                	Uint32 pixel2 = SDL_MapRGB
                        (image_surface->format, 255, 255, 255);
                        put_pixel(image_surface, k, l, pixel2);
                }
        }
	for(int k = 0; k < 20; k++)
    	{
   		for(int l = 0; l < width; l++)
               	{
                	Uint32 pixel2 = SDL_MapRGB
                        (image_surface->format, 255, 255, 255);
                        put_pixel(image_surface, l, k, pixel2);
                }
        }
	for(int k = height-20; k < height; k++)
    	{
   		for(int l = 0; l < width; l++)
               	{
                	Uint32 pixel2 = SDL_MapRGB
                        (image_surface->format, 255, 255, 255);
                        put_pixel(image_surface, l, k, pixel2);
                }
        }
	
SDL_SaveBMP(rotozoomSurface(image_surface,0,1,1),"image3.bmp");
}
//save
                                                  
    image_surface = load_image("image3.bmp");
    // VARIABLES :
    width = image_surface->w;
    height = image_surface->h;
    int size = width * height;
    int *histo = 0;
    histo = (int*) malloc(size * sizeof(int));
    if (histo == NULL)
    {
     printf("Can't allocated memory");
    }
    int * coord = (int*) malloc(2 * sizeof(int));
    histo = initializeHisto(histo, image_surface, width, height);
   // printMatrix(histo, height, width);
    coord = DetectStart(histo, width, height, coord);
    //int * Case = 0;
    //Case = (int*) malloc(size/81 * sizeof(int));
    //if (Case == NULL)
    //{
//      printf("lineDetection : Can't allocated memory");
  //  }
    //Case = getCase(histo, width, height, coord, size, 2, Case);
    //printf("allocated : %d, loop : %d", size/81, ((width-1)/9)*((height-1)/9));
    
printf("avant");
DoneAll(histo, coord, width, height);
printf("apres");
SDL_FreeSurface(image_surface);
void SDL_FreeSurface(SDL_Surface *surface); 
return image_surface;
 }

int main(int argc , char *argv[])
{
    char* image = argv[1];
    SDL_Surface* image_surface;                                                
    init_sdl();                                                                
    image_surface = load_image(image);      
    
	pretraitement(image_surface);
	return 0;
}

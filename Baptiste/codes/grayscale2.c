#include <stdio.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
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


    
int main()                                                                     
{                                                                              
                                                                               
    SDL_Surface* image_surface;                                                
    SDL_Surface* screen_surface;                                               
    init_sdl();                                                                
    //image_surface = load_image("images/image_04.jpeg");      
    image_surface = load_image("images/image01rot.jpg");                        
    screen_surface = display_image(image_surface);                             
                                                                   
    int width = image_surface->w;                                              
    int height = image_surface->h;                                             
    float zoom = 0;                                                            
    float w = image_surface->w;                                                
    float h = image_surface->h;                                                
    float mid = 0;
    int min_gray =0;         
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB                              
    (screen_surface->format, 255, 255, 255));                                  
                                                       
update_surface(screen_surface,image_surface);                               
wait_for_keypressed();
                                                                           
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
     screen_surface = display_image(image_surface);

      width = image_surface->w;
      height = image_surface->h;
     w = image_surface->w;
     h = image_surface->h;
      SDL_FillRect(screen_surface, NULL, SDL_MapRGB
     (screen_surface->format, 255, 255, 255));

                                                  
update_surface(screen_surface,image_surface);                               
wait_for_keypressed();
 
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
update_surface(screen_surface,image_surface);
wait_for_keypressed();

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
                                                  
update_surface(screen_surface,image_surface);                               
wait_for_keypressed();
 
for(int x = 0; x < width; x++)
    {
       for(int y = 0; y < height;y++)
       {
         Uint32 pixel = get_pixel(image_surface, x, y);
         Uint8 r, g, b;
         SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
	 Uint8 c = f(r,2);	
	 Uint32 pixelco = SDL_MapRGB(image_surface->format, c, c, c);
	 put_pixel(image_surface, x, y, pixelco);
	}
    }
update_surface(screen_surface,image_surface);
wait_for_keypressed();
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
                                                  
update_surface(screen_surface,image_surface);                               
wait_for_keypressed();
 
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
	screen_surface = display_image(image_surface);                         
                                                                               
	width = image_surface->w;                                              
	height = image_surface->h;                       
	SDL_FillRect(screen_surface, NULL, SDL_MapRGB              
	(screen_surface->format, 255, 255, 255));                            
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
                                                  
update_surface(screen_surface,image_surface);                               
wait_for_keypressed();
//Noise suppression

	SDL_SaveBMP(rotozoomSurface(image_surface,0,1,1),"image3.bmp");
        image_surface = load_image("image3.bmp");
        screen_surface = display_image(image_surface);
	y = height/2;	
	while (white==0)
	{
		printf("b");
		Uint32 pixel = get_pixel(image_surface, x, y);
		SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
		x+=1;
		if(r == 255){white = 1;}	
	}
	// Waiting for one white pixel
	
	while (black==0 && !(line(image_surface,x,y,0)))
	{
		x+=1;
		printf("n");
		Uint32 pixel = get_pixel(image_surface, x, y);
		SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
		if(r == 0){black = 1;}	
	}

	//Waiting for one black pixel
	double angleline = 0;
	double linea = 0;
	 while(!(lignedroite(image_surface,x,y,0)))
         {	
		angleline += 1;
		printf("rentre dans boucle.      %d       ",x);
		y = height/2;
		SDL_SaveBMP(rotozoomSurface(image_surface,1,1,1),"image3.bmp");
		image_surface = load_image("image3.bmp");                      
		screen_surface = display_image(image_surface);             
        	if(lignedroite(image_surface,x,y,0)){printf("  wtfff???  ");} 
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
                 		printf("b");
                 		Uint32 pixel = get_pixel(image_surface, x, y);
                 		SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
                 		x+=1;
                 		if(r == 255){white = 1;}
      			}
			while (black==0 && !(line(image_surface,x,y,0)))
			{
				x+=1;
				printf("n");
				Uint32 pixel = get_pixel(image_surface, x, y);
				SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
				if(r == 0){black = 1;}	
			}
			while(r != 0)
			{	
				printf( " x+1 ");
				x+=1;
				Uint32 pixel = get_pixel(image_surface, x, y);
   	        		SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
			}//adjust if the rotation loose the line
		}
		
	printf("%d",x);
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
SDL_SaveBMP(rotozoomSurface(image_surface,0,1,1),"image3.bmp");
//save
                                                  
update_surface(screen_surface,image_surface);                               
wait_for_keypressed();
SDL_FreeSurface(image_surface);
SDL_FreeSurface(screen_surface);
void SDL_FreeSurface(SDL_Surface *surface);

    return 0;
 }

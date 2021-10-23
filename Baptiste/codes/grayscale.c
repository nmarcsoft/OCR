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
    screen = SDL_SetVideoMode(1000, 1000, 32, SDL_HWSURFACE);
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

//Threshold

/*long double threshold()
{
	SDL_Surface* image_surface;
     	init_sdl();
	image_surface = load_image("images/image01rot.jpg");
	//float k = 0.5;
	int x = 0;
	int y = 0;
	long double mean = 0;
	long double var = 0;
	long double sumvar = 0; 
	long double stdev = 0;
	long double mid = 0;
	int pasx = 25;
	int pasy = 25;
	for (; x < x+pasx; x++) //Mean
    	{	
		while((x+pasx)>image_surface->w)
		{
			pasx -=1;
		}
        	for(; y < y+pasy;y++)
        	{
			while((x+pasy)>image_surface->h)
               		{
                        	pasy -=1;
                	}
			Uint32 pixel = get_pixel(image_surface, x, y);
       			Uint8 r, g, b;
       			SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
       			mid = 0.3*r + 0.59*g + 0.11*b;
			mean = mean+mid;
			
		}
	}
	mean = mean / 625;
	x = 0;
	y = 0;
	for (; x < x+25; x++)  //Variance and standard deviation
        {
                for(; y < y+25;y++)
                {
			Uint32 pixel = get_pixel(image_surface, x, y);
                        Uint8 r, g, b;
                        SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
                        mid = 0.3*r + 0.59*g + 0.11*b;
			var = (mid-mean)*(mid-mean);
        		sumvar = sumvar+var;
		}
	}
        stdev = sqrtf(sumvar/(625-1));
	printf("%.6Lf", stdev);
	
}
*/



int main() 
{

    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    init_sdl();
    image_surface = load_image("images/image_06.jpeg");
    screen_surface = display_image(image_surface);


    SDL_FillRect(screen_surface, NULL, SDL_MapRGB
        (screen_surface->format, 255, 255, 255));
    int width = image_surface->w;
    int height = image_surface->h;
    int zoom = 1;
    if(height > width)
    {zoom = 1000/height;}
    else
    {zoom = 1000/width;}
    rotozoomSurface(image_surface,1,zoom,1);
    
    
    float mid = 0;
    long double min_gray = 255;
    update_surface(screen_surface, image_surface);

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
update_surface(screen_surface, image_surface);
wait_for_keypressed();
printf("Debug1\n");
/*int i =0;
int j = 0;
int r = 0;
int x= 0;
for (; i < height; i++)
    {
      for (; j < width; j++)
      {
        Uint32 pixel = get_pixel(image_surface, j, i);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        if(r==0)
        {break;}
      }
      if(r==0)
      { x=i;
	      break;}
    }
printf("%d",x);
while(angletorotate == 0 && x>0) 
{
Uint32 pixel2 = get_pixel(image_surface, x, j+20);
Uint8 r2, g2, b2;
SDL_GetRGB(pixel2, image_surface->format, &r2, &g2, &b2);

angletorotate = (j+20)/(i-x);
printf("%.2f",angletorotate);
x--;
}*/


    float k = 0.5;
    // calcul de R = o max

    float T = 0;
    int tottemp = 0;
    int moytemp = 0;
    float var = 0;
    float maxvar = 0;


	
for (int tempw = 0; tempw<(width-25); tempw+=25)
{
	for(int temph = 0; temph<(height-25); temph+=25)
	{
		for(int x = tempw ;x<tempw+24;x++)
		{
			for(int y = temph; y<temph+24;y++)
			{
				Uint32 pixel = get_pixel(image_surface, x, y);
        	        	Uint8 r, g, b;
	                	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
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
		T = (1-k)*moytemp + k*min_gray + k* (var/(maxvar*(moytemp-min_gray)));
		for(int x = tempw ;x<tempw+24;x++)
                {
                        for(int y = temph; y<temph+24;y++)
                        {
                                Uint32 pixel = get_pixel(image_surface, x, y);
                                Uint8 r, g, b;
                                SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                     		if (r < T)
                		{
					Uint32 pixel2 = SDL_MapRGB(image_surface->format, 0, 0, 0);
  					put_pixel(image_surface, x, y, pixel2);
				}
        		        else
               			{
					Uint32 pixel2 = SDL_MapRGB(image_surface->format, 255, 255, 255);
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
    update_surface(screen_surface,image_surface);

    SDL_Surface *rotation = NULL;
    SDL_Event event;
    SDL_Rect rect;
    double angle = 0;
	printf("etici ?");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_WM_SetCaption("Faire des rotations avec SDL_gfx", NULL);

    int continuer = 1;
    while(continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
		continuer = 0;
                break;
        }

	 //On augmente l'angle pour que l'image tourne sur elle-même.

	rotation = rotozoomSurface(image_surface, angle, 0.5, 1); 
	//On transforme la surface image.
	rect.x =  500 - rotation->w / 2;
	rect.y =  500 - rotation->h / 2;
	//On transforme la surface image.
        //On positionne l'image en fonction de sa taille.
        SDL_BlitSurface(rotation , NULL, screen_surface, &rect); 
	//On affiche la rotation de la surface image.
        SDL_FreeSurface(rotation);
	//On efface rotation car on va la redéfinir dans la prochaine boucle. Si on ne le fait pas, cela crée une fuite de mémoire. 
	        
	SDL_Flip(screen_surface);
	angle+=2;
        SDL_Flip(screen_surface);




    update_surface(screen_surface,image_surface);
    wait_for_keypressed();
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface); 



	int a = getchar();
	if (a == 97)
	{
	continuer = 0;
	}
    }

SDL_SaveBMP(image_surface,"imagemodif2.bmp" );



//Rotate function
//https://openclassrooms.com/forum/sujet/nouvelles-dimensions-image-avec-sdl-gfx
//SDL_Surface * zoomSurface ( SDL_Surface *src, double zoomx, double zoomy, int smooth);
SDL_SaveBMP(rotozoomSurface(image_surface,1,0.5,1),"imagemodif3.bmp");
update_surface(screen_surface, image_surface);
SDL_FreeSurface(image_surface);
SDL_FreeSurface(screen_surface);
void SDL_FreeSurface(SDL_Surface *surface);

    return 0;
 }

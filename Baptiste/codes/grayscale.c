#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <SDL/SDL_rotozoom.h>

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
    SDL_Surface* screen_surface;
    init_sdl();

    image_surface = load_image("images/image_04.jpeg");
    screen_surface = display_image(image_surface);
    int width = image_surface->w;
    int height = image_surface->h;
    int max = 0;
    int min = 255;
    int mid = 0;
    //average image color calcul
    for(int x = 0; x < width; x++)
    {
 	for(int y = 0; y < height;y++)
 	{
 	      Uint32 pixel = get_pixel(image_surface, x, y);
 	      Uint8 r, g, b;
 	      SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
 	      mid = 0.3*r + 0.59*g + 0.11*b;
 	      if (mid <min)
 	      {min = mid;}
 	      if(mid > max)
 	      {max = mid;}
 	      Uint32 pixel2 = SDL_MapRGB(image_surface->format, mid, mid, mid);
 	      put_pixel(image_surface, x, y, pixel2);
 	}
    }
    int threshold = ((max + mid) / 2)-max;
    for (int i = 0; i < width; i++)
    {
      for (int j = 0; j < height; j++)
      {
	Uint32 pixel = get_pixel(image_surface, i, j);
	Uint8 r, g, b;
	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	Uint8 t = (r + g + b)/3;
	if(t>threshold)
	{
	   Uint32 pixel2 = SDL_MapRGB(image_surface->format, 255, 255, 255);
	   put_pixel(image_surface, i, j, pixel2);
	}
	else
	{
	   Uint32 pixel2 = SDL_MapRGB(image_surface->format, 0, 0, 0);
	   put_pixel(image_surface, i, j, pixel2);
	}
      }
    }

    SDL_Surface *rotation = NULL;
    SDL_Event event;
    SDL_Rect rect;
    double angle = 0;

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

	SDL_FillRect(screen_surface, NULL, SDL_MapRGB
			(screen_surface->format, 255, 255, 255));
	rotation = rotozoomSurface(image_surface, angle, 1.0, 1); 
	//On transforme la surface image.
	rect.x =  500 - rotation->w / 2;
	rect.y =  500 - rotation->h / 2;
	rotation = rotozoomSurface(image_surface, angle, 1.0, 1);
	//On transforme la surface image.
        //On positionne l'image en fonction de sa taille.
        SDL_BlitSurface(rotation , NULL, screen_surface, &rect); 
	//On affiche la rotation de la surface image.
        SDL_FreeSurface(rotation);
	//On efface rotation car on va la redéfinir dans la prochaine boucle. Si on ne le fait pas, cela crée une fuite de mémoire. 
        
	SDL_Flip(screen_surface);
	angle+=2;
        SDL_Flip(screen_surface);
	if(getchar() == 97)
	{continuer = 0;}	
    }
	SDL_SaveBMP(image_surface,"imagemodif2.bmp" );


	//Rotate function
	double zoomw = 1080/width;

	SDL_SaveBMP(rotozoomSurface(image_surface,1,zoomw,1),"imagemodif3.bmp");
	update_surface(screen_surface, image_surface);
	SDL_FreeSurface(image_surface);
    	SDL_FreeSurface(screen_surface);
	void SDL_FreeSurface(SDL_Surface *surface);

    return 0;
}

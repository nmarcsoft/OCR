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

int main(){
   SDL_Surface* image_surface;
   init_sdl();
   image_surface = load_image("coucou.png");
   int width = image_surface->w;
   int height = image_surface->h;
   int * histo = 0;
   int size = width * height;
   histo = (int*) malloc(size * sizeof(int));
   histo = initializeHisto(histo, image_surface, width, height);
   printf("%d", *(histo));
   return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
int X[784][1];
double W1[10][784];
double b1[10];
double W2[10][10];
double b2[10];
double Z1[10][1];
double A1[10][41450];
double A1_t[41000][10];
double Z2[10][41450];
double A2[10][41450];
double Z2exp[10][1];
double sum_Z2exp[1];
int predictions[1];

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

void printMatrix(int* a, int height, int width)
{
/*
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
*/
   int size = 784;
   for (int i = 0; i < size; i++)
   {
   	printf("%d ", *(a + i));
	if (i % 28 == 0)
	{
	printf("\n");
	}
   }
   printf("}");
   printf("\n\n");
}

void RelU(){
    for(int i=0; i < 10; i++){
        for(int j = 0; j < 1; j++){
            if(Z1[i][j] > 0){
                A1[i][j] = Z1[i][j];
            }
            else{
                A1[i][j] = 0;
            }
        }
    }
}

void add(int v){
    if(v == 0){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 1; j++){
                Z1[i][j] += b1[i];
            }
        }
    }
    if(v == 1){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 1; j++){
                Z2[i][j] += b2[i];
            }
        }
    }

}

void softmax(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 1; j++){
            Z2exp[i][j] = exp(Z2[i][j]);
        }
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 1; j++){
            sum_Z2exp[j] = 0;
        }
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 1; j++){
            sum_Z2exp[j] += Z2exp[i][j];
        }
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 1; j++){
            A2[i][j] = exp(Z2[i][j]) / sum_Z2exp[j];
        }
    }
}

void dot(int R1, int C2, int R2, int v){
    double hlo;
    double ow;
    if(v == 0){
        double tmp;
        for(int x = 0; x < R1; x++){
            for(int j = 0; j < C2; j++){
                Z1[x][j] = 0;
                for(int k = 0; k < R2; k++){
                    hlo = W1[x][k];
                    ow = X[k][j];
                    tmp = hlo * ow;
                    if(isnan(tmp) || isinf(tmp)){
                        tmp = 0;
                    }
                    Z1[x][j] += tmp;
                }
            }
        }
    }
    if(v == 1){
        double tmp;
        for(int x = 0; x < R1; x++){
            for(int j = 0; j < C2; j++){
                Z2[x][j] = 0;
                for(int k = 0; k < R2; k++){
                    hlo = W2[x][k];
                    ow = A1[k][j];
                    tmp = hlo * ow;
                    if(isnan(tmp) || isinf(tmp)){
                        tmp = 0;
                    }
                    Z2[x][j] += tmp;
                }
            }
        }
    }
}


void bias1(int i, char *line){
    b1[i] = strtod(line, NULL);
}

void weight1(int i, int j, char* line){
    W1[i][j] = strtod(line, NULL);
}

void weight2(int i, int j, char* line){
    W2[i][j] = strtod(line, NULL);
}

void bias2(int i, char *line){
    b2[i] = strtod(line, NULL);
}

void forward_prop(){
    dot(10, 1, 784, 0); //Z1 = W1.dot(X)
    add(0);
    RelU();
    dot(10, 1, 10, 1);
    add(1);
    softmax();
}
void get_prediction(){
    double maxi = 0;
    int max_indice = -1;
    for(int j =0; j < 1; j++){
        for(int i = 1; i < 10; i++){
            if(maxi < A2[i][j]){
                maxi = A2[i][j];
                max_indice = i;
            }
        }
        maxi = 0;
        predictions[j] = max_indice;
    }    
}

void find_image(int *image){
    for(int i = 0; i < 754; i++){
        X[i][0] = *(image + i);
    }
}

int do_prediction(int *image){
    find_image(image);
    forward_prop();
    get_prediction();
    return predictions[0]; 
}

int main(){
    FILE *fp;
    char *line;
    size_t len = 0;
    ssize_t read;
    // SDL //
    SDL_Surface* image_surface;
    init_sdl();
    image_surface = load_image("image.bmp");
    int width = image_surface->w;
    int height = image_surface->h;
    int * histo = 0;
    histo = (int*) malloc( (width * height) * sizeof(int));
    histo = initializeHisto(histo, image_surface, width, height);
    // END SDL // 
    
    fp = fopen("save.txt", "r");
    if(fp == NULL){
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 784; j++){
            read = getline(&line, &len, fp);
            weight1(i, j, line);
        }
    }
    for(int i = 0; i <10; i++){
        read = getline(&line, &len, fp);
        bias1(i, line);
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            read = getline(&line, &len, fp);
            weight2(i, j, line);
        }
    }
    for(int i = 0; i < 10; i++){
        read = getline(&line, &len, fp);
        bias2(i,line);
    }
    fclose(fp);
    int predi = do_prediction(histo);
    printf("%i", predi);
    return 0;

}


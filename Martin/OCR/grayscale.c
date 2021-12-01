#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "mnist_file.h"
#include "nn.h"
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



void weights(int i, int j, char *line, neural_network_t *network){
    network->weights[i][j] = strtod(line, NULL);
}

void bias(int i, char *line, neural_network_t *network){
    network->bias[i] = strtod(line, NULL);
}

float calculate_accuracy(mnist_dataset_t * dataset, neural_network_t * network)
{
    float activations[MNIST_LABELS], max_activation;
    int i, j, correct, predict;
    for (i = 0, correct = 0; i < dataset->size; i++) {
	hypothesis(&dataset->images[i], network, activations);
        for (j = 0, predict = 0, max_activation = activations[0]; j < MNIST_LABELS; j++) {
            if (max_activation < activations[j]) {
                max_activation = activations[j];
                predict = j;
            }
        }
        if (predict == dataset->labels[i]) {
            correct++;
        }
    }
    return ((float) correct) / ((float) dataset->size);
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



int main(){
    float accuracy;
    const char *test_images_file = "data/t10k-images-idx3-ubyte";
    const char *test_labels_file = "data/t10k-labels-idx1-ubyte";
    neural_network_t network;
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
    // SDL // 
    fp = fopen("program.txt", "r");
    if(fp == NULL){
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i <10; i++){
        read = getline(&line, &len, fp);
        bias(i, line, &network);
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 784; j++){
            read = getline(&line, &len, fp);
            weights(i, j, line, &network);
        }
    }
    fclose(fp);
    mnist_dataset_t *test_dataset;
    test_dataset = mnist_get_dataset(test_images_file, test_labels_file);
    accuracy = calculate_accuracy(test_dataset, &network);
    printf("Accuracy: %.3f\n", accuracy);
    //PREDICT
    int predict = 0;
    float activations[MNIST_LABELS], max_activations;
    //hypothesis(&test_dataset->images[0], &network, activations);
    hypothesis_real(histo, &network, activations);
    max_activations = activations[0];
        for(int z = 0; z < 10; z++){
		printf("%f ", activations[z]);
            if(max_activations < activations[z]){
                max_activations = activations[z];
                predict = z;
            }
        }
    printMatrix(histo, 28, 28);
    printf("%d", predict);   
    return 0;
}

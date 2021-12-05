#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include <stdlib.h>
#include <SDL.h>



int do_prediction(int *image);
void find_image(int *image);
 void get_prediction();
void forward_prop();
void bias2(int i, char *line);
void weight2(int i, int j, char* line);
void weight1(int i, int j, char* line);
void bias1(int i, char *line);
void dot(int R1, int C2, int R2, int v);
void softmax();
 void add(int v);
void RelU();
int * initializeHisto(int * histo, SDL_Surface* image_surface, int widt    h, int height);
void copySurface(int * from, SDL_Surface* to, int width, int height);






#endif

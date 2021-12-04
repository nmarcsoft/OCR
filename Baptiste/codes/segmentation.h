#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include <stdlib.h>
#include <SDL.h>

void copySurface(int * from, SDL_Surface* to, int width, int height);
int * DetectStart(int * histo, int width, int height, int * coord);
int * getCase(int * histo, int width, int height, int * coord, int size,     int num, int * Case);
int * initializeHisto(int * histo, SDL_Surface* image_surface, int width    , int height);
void printMatrix(int* a, int height, int width);
int Width(int * histo, int * coord, int width);
int Height(int * histo, int * coord, int width);
 int * cut(int * histo, int i, int j, int widthR, int heightR, int width,     int * a);
int Rogne(int * toPrint, int width, int height);
void DoneAll(int * histo, int * coord, int width, int height);






#endif

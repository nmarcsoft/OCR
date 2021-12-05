#ifndef GRAYSCALE2_H_
#define GRAYSCALE2_H_

#include <stdlib.h>
#include <SDL.h>
Uint8 f(Uint8 c, double n);
int lignedroite (SDL_Surface* image_surface,int i,int j,int toreturn);
int lignehorizon (SDL_Surface* image_surface,int i,int j,int toreturn);
int line (SDL_Surface* image_surface,int i,int j,int toreturn);
int pretraitement(SDL_Surface* image_surface, int toStop);
int NicoMartin(SDL_Surface* image_surface,int toStop);

#endif

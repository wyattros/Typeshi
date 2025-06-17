#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int init();
SDL_Window* create_window(int h, int w);

#endif
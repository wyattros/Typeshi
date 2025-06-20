#ifndef SDL_MANAGER_H
#define SDL_MANAGER_H

#include "audio_processing.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int sdl_init();
int sdl_quit(AudioData* ad, SDL_Renderer* renderer, SDL_Window* window);
SDL_Window* create_window(int h, int w);
SDL_Renderer* create_renderer(SDL_Window* window);

#endif
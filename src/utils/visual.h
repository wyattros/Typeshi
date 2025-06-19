#ifndef VISUAL_H
#define VISUAL_H

#include <SDL2/SDL.h>

int draw_bar(SDL_Renderer* renderer, float* input_samples, SDL_Color color, int count);
SDL_Color fade(int* first_color, int* second_color, int count);
int draw_visual(SDL_Renderer* renderer, float* input_samples, int enable_fade, int* first_color, int* second_color);

#endif
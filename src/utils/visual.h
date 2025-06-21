#ifndef VISUAL_H
#define VISUAL_H

#include <SDL2/SDL.h>
#include <math.h>

#define RADIUS 200
#define PI 4.0 * atan(1.0)
#define DEFAULT_GAP 0

extern int GAP;

typedef struct point
{
    int x;
    int y;
} point;

void draw_rectangle(SDL_Renderer* renderer, point a, point b, point c, point d);
point rotate(point center, point p, double angle);
void draw_bar_circular(SDL_Renderer* renderer, float* input_samples, SDL_Color color, int count);
void draw_visual_circular(SDL_Renderer* renderer, float* input_samples, int enable_fade, int* first_color, int* second_color);
void draw_bar(SDL_Renderer* renderer, float* input_samples, SDL_Color color, int count);
SDL_Color fade(int* first_color, int* second_color, int count);
void draw_visual(SDL_Renderer* renderer, float* input_samples, int enable_fade, int* first_color, int* second_color);

#endif
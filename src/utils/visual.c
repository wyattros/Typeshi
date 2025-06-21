#include "sdl_manager.h"
#include "audio_processing.h"
#include "visual.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

void draw_rectangle(SDL_Renderer* renderer, point a, point b, point c, point d)
{
    SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
    SDL_RenderDrawLine(renderer, b.x, b.y, c.x, c.y);
    SDL_RenderDrawLine(renderer, c.x, c.y, d.x, d.y);
    SDL_RenderDrawLine(renderer, d.x, d.y, a.x, a.y);
}

void fill_rectangle(SDL_Renderer* renderer, point a, point b, point c, point d, SDL_Color color)
{
    SDL_Vertex vertices[6] = {
        { { a.x, a.y }, color, {0.0f, 0.0f} },
        { { b.x, b.y }, color, {0.0f, 0.0f} },
        { { c.x, c.y }, color, {0.0f, 0.0f} },
        { { a.x, a.y }, color, {0.0f, 0.0f} },
        { { c.x, c.y }, color, {0.0f, 0.0f} },
        { { d.x, d.y }, color, {0.0f, 0.0f} }
    };
    SDL_RenderGeometry(renderer, NULL, vertices, 6, NULL, 0);
}

point rotate(point center, point p, double angle)
{
    point res = 
    {
        .x = center.x + cos(angle) * (p.x - center.x) - sin(angle) * (p.y - center.y),
        .y = center.y + sin(angle) * (p.x - center.x) + cos(angle) * (p.y - center.y)
    };

    return res;
}

void draw_bar_circular(SDL_Renderer* renderer, float* input_samples, SDL_Color color, int count)
{
    double angle = 2*PI / (double) RECTANGLE_COUNT;
    int perimeter = 2*PI*RADIUS;

    int rectangle_count = RECTANGLE_COUNT;
    if (RECTANGLE_COUNT > perimeter)
        rectangle_count = perimeter;

    point center = {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
    point origin_a = {center.x - (PI*RADIUS)/rectangle_count, center.y - RADIUS};
    point origin_b = {center.x - (PI*RADIUS)/rectangle_count + perimeter/rectangle_count, center.y - RADIUS};
    point origin_c = {center.x - (PI*RADIUS)/rectangle_count + perimeter/rectangle_count, center.y - RADIUS - input_samples[count] * AMPLIFY_POWER};
    point origin_d = {center.x - (PI*RADIUS)/rectangle_count, center.y - RADIUS - input_samples[count] * AMPLIFY_POWER};

    point a = rotate(center, origin_a, angle*count);
    point b = rotate(center, origin_b, angle*count);
    point c = rotate(center, origin_c, angle*count);
    point d = rotate(center, origin_d, angle*count);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    fill_rectangle(renderer, a, b, c, d, color);
}

void draw_visual_circular(SDL_Renderer* renderer, float* input_samples, int enable_fade, int* first_color, int* second_color)
{
    SDL_Color color = {
        .r = (Uint8)first_color[0],
        .g = (Uint8)first_color[1],
        .b = (Uint8)first_color[2],
        .a = 255
    };

    for (int i = 0; i < RECTANGLE_COUNT; i++)
    {
        if (enable_fade)
        {
            if (i < RECTANGLE_COUNT/2)
                color = fade(first_color, second_color, i);
            else
                color = fade(second_color, first_color, i);
        }
        draw_bar_circular(renderer, input_samples, color, i);
    }
}

void draw_bar(SDL_Renderer* renderer, float* input_samples, SDL_Color color, int count)
{
    if (WINDOW_WIDTH / RECTANGLE_COUNT * count + (count > 0 ? GAP * count : 0) > WINDOW_WIDTH)
        return;
        
    SDL_Rect rect = {
        .x = WINDOW_WIDTH / RECTANGLE_COUNT * count + (count > 0 ? GAP * count : 0),
        .w = WINDOW_WIDTH / RECTANGLE_COUNT,
        .h = input_samples[count] * AMPLIFY_POWER,
        .y = WINDOW_HEIGHT - input_samples[count] * AMPLIFY_POWER
    };    

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Color fade(int* first_color, int* second_color, int count)
{
    int res[3] = {
        first_color[0],
        first_color[1],
        first_color[2]
    };

    int diff_color[3] = {
        second_color[0] - first_color[0],
        second_color[1] - first_color[1],
        second_color[2] - first_color[2]
    };

    for (int i = 0; i < 3; i++)
    {
        res[i] += count * diff_color[i] / RECTANGLE_COUNT;
    }

    SDL_Color fade_color = {
        .r = (Uint8)res[0],
        .g = (Uint8)res[1],
        .b = (Uint8)res[2],
        .a = 255
    };

    return fade_color;
}

void draw_visual(SDL_Renderer* renderer, float* input_samples, int enable_fade, int* first_color, int* second_color)
{
    SDL_Color color = {
        .r = (Uint8)first_color[0],
        .g = (Uint8)first_color[1],
        .b = (Uint8)first_color[2],
        .a = 255
    };

    for (int i = 0; i < RECTANGLE_COUNT; i++)
    {
        if (enable_fade)
            color = fade(first_color, second_color, i);
            
        draw_bar(renderer, input_samples, color, i);
    }
}

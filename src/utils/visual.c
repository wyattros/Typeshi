#include "sdl_manager.h"
#include "audio_processing.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int draw_bar(SDL_Renderer* renderer, float* input_samples, SDL_Color color, int count)
{
    SDL_Rect rect = {
        .x = WINDOW_WIDTH / RECTANGLE_COUNT * count,
        .w = WINDOW_WIDTH / RECTANGLE_COUNT,
        .h = input_samples[count] * AMPLIFY_POWER,
        .y = WINDOW_HEIGHT - input_samples[count] * AMPLIFY_POWER
    };    

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);

    return 0;
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

int draw_visual(SDL_Renderer* renderer, float* input_samples, int enable_fade, int* first_color, int* second_color)
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

    return 0;
}

#include "audio_processing.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int sdl_init()
{
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
    {
        printf("Could not initialize SDL.\n %s\n", SDL_GetError());
        return 1;
    }

    printf("SDL initialized.\n");
    return 0;
}

int sdl_quit(AudioData* ad)
{
    SDL_CloseAudio();
    SDL_FreeWAV(ad->buffer);
    SDL_Quit();

    return 0;
}

SDL_Window* create_window(int h, int w)
{
    SDL_Window* window = SDL_CreateWindow("Typesh!",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        h, w, 0);

    return window;
}

SDL_Renderer* create_renderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return renderer;
}

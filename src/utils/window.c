#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int init()
{
    int process_code = SDL_Init(SDL_INIT_EVERYTHING);

    if(process_code)
        printf("SDL initialized.");
    else
        printf("Could not initialize SDL.\nError code: %i", process_code);
    return process_code;
}

SDL_Window* create_window(int h, int w)
{
    SDL_Window* window = SDL_CreateWindow("Typesh!",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        h, w, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return window;
}

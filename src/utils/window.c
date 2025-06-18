#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int init()
{
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
    {
        printf("Could not initialize SDL.\n %s\n", SDL_GetError());
        return -1;
    }

    printf("SDL initialized.\n");
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

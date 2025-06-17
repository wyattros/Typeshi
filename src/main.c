#include "utils/window.h"
#include <SDL2/SDL.h>

int main()
{
    int init_code = init();

    if (init_code)
        return init_code;

    SDL_Window* window = create_window(1920, 1080);

    int running = 1;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_Delay(16);
    }

    return 0;
}
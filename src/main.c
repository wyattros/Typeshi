#include "utils/sdl_manager.h"
#include "utils/audio_processing.h"
#include "utils/visual.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

UserData ud;
int16_t temp_samples[SAMPLE_SIZE];
float input_samples[RECTANGLE_COUNT];
float smoothed_samples[RECTANGLE_COUNT];

int main()
{
    int init_code = sdl_init();
    if (init_code)
        return init_code;

    AudioData ad;
    int load_code = load_file("faint.wav", &ud, &ad);
    if (load_code)
        return load_code;

    SDL_Window* window = create_window(WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_Renderer* renderer = create_renderer(window);

    SDL_PauseAudio(0);

    int running = 1;
    SDL_Event event;

    while (running) 
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = 0;
                sdl_quit(&ad);
            }
        }

        copy_audio_buffer(&ud, temp_samples);
        create_samples(smoothed_samples, input_samples, temp_samples);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int first_color[3] = {255, 0, 0};
        int second_color[3] = {255, 165, 0};

        draw_visual(renderer, smoothed_samples, 1, first_color, second_color);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
#include "utils/sdl_manager.h"
#include "utils/audio_processing.h"
#include "utils/visual.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

UserData ud;
int16_t temp_samples[SAMPLE_SIZE];
int RECTANGLE_COUNT = DEFAULT_RECTANGLE_COUNT;
int GAP = DEFAULT_GAP;

int main(int argc, char** argv)
{
    if (argc <= 1)
        return 1;

    int init_code = sdl_init();
    if (init_code)
        return init_code;

    int fade = 0;
    int circular = 0;

    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
            circular = 1;
        if (strcmp(argv[i], "-f") == 0)
            fade = 1;
        if (strcmp(argv[i], "-b") == 0)
            RECTANGLE_COUNT = atoi(argv[i+1]);
        if (strcmp(argv[i], "-g") == 0)
            GAP = atoi(argv[i+1]);
    }

    float input_samples[RECTANGLE_COUNT];
    float smoothed_samples[RECTANGLE_COUNT];

    for (int i = 0; i < RECTANGLE_COUNT; i++) {
        input_samples[i] = 0.0f;
        smoothed_samples[i] = 0.0f;
    }

    AudioData ad;
    int load_code = load_file(argv[1], &ud, &ad);
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
            }
        }

        copy_audio_buffer(&ud, temp_samples);
        create_samples(smoothed_samples, input_samples, temp_samples);

        SDL_SetRenderDrawColor(renderer, 0, 20, 0, 255);
        SDL_RenderClear(renderer);

        int first_color[3] = {255, 0, 0};
        int second_color[3] = {255, 255, 0};

        if (circular)
            draw_visual_circular(renderer, smoothed_samples, fade, first_color, second_color);
        else
            draw_visual(renderer, smoothed_samples, fade, first_color, second_color);
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    sdl_quit(&ad, renderer, window);
    
    return 0;
}
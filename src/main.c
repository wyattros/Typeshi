#include "utils/window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#define BUFFER_SIZE 8192
#define SAMPLE_SIZE 1920
#define RECTANGLE_COUNT (1920)
#define AVG_COUNT (SAMPLE_SIZE / RECTANGLE_COUNT)
#define AMPLIFY_POWER 500.0f

Uint8* audio_pos;
Uint32 audio_len;
int16_t buffer[BUFFER_SIZE];
int idx = 0;
int16_t temp_samples[SAMPLE_SIZE];
float input_samples[RECTANGLE_COUNT];

void audio_callback(void* userdata, Uint8* stream, int len)
{
    if (audio_len == 0)
        return;

    len = ( len > audio_len ? audio_len : len );
    SDL_memcpy(stream, audio_pos, len);

    audio_pos += len;
    audio_len -= len;

    int sample_count = len / sizeof(int16_t);
    int16_t* input = (int16_t*)stream;
    for(int i = 0; i < sample_count; i++)
    {
        buffer[idx] = input[i];
        idx++;
        idx %= BUFFER_SIZE; 
    }

}

int main()
{
    float smoothed_samples[RECTANGLE_COUNT] = {0};
    float smoothing_factor = 0.005f;

    int init_code = init();

    if (init_code != 0)
        return init_code;

    /*int devices_no = SDL_GetNumAudioDevices(0);
    printf("%i\n", devices_no);
    for(int i = 0; i < devices_no; i++)
    {
        printf(SDL_GetAudioDeviceName(i, 0));
    }*/

	static Uint32 wav_length;
	static Uint8 *wav_buffer;
	static SDL_AudioSpec wav_spec;

    char* file = "faint.wav";

	if(SDL_LoadWAV(file, &wav_spec, &wav_buffer, &wav_length) == NULL)
        return -1;

	wav_spec.callback = audio_callback;
	wav_spec.userdata = NULL;
	audio_pos = wav_buffer;
	audio_len = wav_length;
	
	SDL_OpenAudio(&wav_spec, NULL);

    SDL_Window* window = create_window(1920, 1080);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //SDL_SetWindowOpacity(window, 0.5);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_PauseAudio(0);

    int running = 1;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
                SDL_CloseAudio();
                SDL_FreeWAV(wav_buffer);
            }
        }

        int start_index = (idx - SAMPLE_SIZE + BUFFER_SIZE) % BUFFER_SIZE;
        if (start_index + SAMPLE_SIZE < BUFFER_SIZE)
            memcpy(temp_samples, &buffer[start_index], SAMPLE_SIZE*sizeof(int16_t));
        else
        {
            int first_half = BUFFER_SIZE - start_index;
            memcpy(temp_samples, &buffer[start_index], first_half*sizeof(int16_t));
            memcpy(&temp_samples[first_half], buffer, (SAMPLE_SIZE-first_half)*sizeof(int16_t)); 
        }


        for (int i = 0; i < RECTANGLE_COUNT; i++) {
            float sum = 0.0f;
            for (int j = 0; j < AVG_COUNT; j++) {
                int sample = temp_samples[i * AVG_COUNT + j];
                sum += fabs(sample / 32768.0f);
            }
            input_samples[i] = sum / AVG_COUNT;

            smoothed_samples[i] = smoothed_samples[i] + smoothing_factor * (input_samples[i] - smoothed_samples[i]);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        int first_color[3] = {255, 0, 0};
        int second_color[3] = {255, 165, 0};
        int diff_collor[3] = {second_color[0] - first_color[0], second_color[1] - first_color[1], second_color[2] - first_color[2]};
        int fade_threshold = 1;
        SDL_SetRenderDrawColor(renderer, first_color[0], first_color[1], first_color[2], 255);
        int count = 0;
        for(int i = 0; i < RECTANGLE_COUNT; i++)
        {
            SDL_Rect rect;
            rect.x = 1920 / RECTANGLE_COUNT * i;
            rect.w = 1920 / RECTANGLE_COUNT;
            rect.h = smoothed_samples[i] * AMPLIFY_POWER;
            rect.y = 1080 - rect.h;         
            SDL_RenderFillRect(renderer, &rect);
            
            if (i >= RECTANGLE_COUNT/diff_collor[1]*count)
            {
                //first_color[0] += fade_threshold * (diff_collor[0] >= 0 ? 1 : 0);
                first_color[1] += fade_threshold * (diff_collor[1] >= 0 ? 1 : 0);
                //first_color[2] += fade_threshold * (diff_collor[2] >= 0 ? 1 : 0);
                count++;
            }
            SDL_SetRenderDrawColor(renderer, first_color[0], first_color[1], first_color[2], 255);
        }
        SDL_RenderPresent(renderer);
    }

    return 0;
}
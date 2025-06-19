#include "audio_processing.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

void audio_callback(void* userdata, Uint8* stream, int len)
{
    UserData* ud = (UserData*)userdata;

    if (ud->position == 0)
        return;

    len = ( len > ud->length ? ud->length : len );
    SDL_memcpy(stream, ud->position, len);

    ud->position += len;
    ud->length -= len;

    int sample_count = len / sizeof(int16_t);
    int16_t* input = (int16_t*)stream;
    for(int i = 0; i < sample_count; i++)
    {
        ud->buffer[ud->idx] = input[i];
        ud->idx++;
        ud->idx %= BUFFER_SIZE; 
    }
}

int load_file(char* file_path, UserData* ud, AudioData* ad)
{
	if(SDL_LoadWAV(file_path, &(ad->spec), &(ad->buffer), &(ad->length)) == NULL)
    {
        printf("Could not load the file provided.\n%s", SDL_GetError());
        return 1;
    }    

	ad->spec.callback = audio_callback;
	ad->spec.userdata = ud;
	ud->position = ad->buffer;
	ud->length = ad->length;
	
	SDL_OpenAudio(&(ad->spec), NULL);
    return 0;
}

int copy_audio_buffer(struct userdata* ud, int16_t* temp_samples)
{
    int start_index = (ud->idx - SAMPLE_SIZE + BUFFER_SIZE) % BUFFER_SIZE;
    if (start_index + SAMPLE_SIZE < BUFFER_SIZE)
        memcpy(temp_samples, &(ud->buffer)[start_index], SAMPLE_SIZE*sizeof(int16_t));
    else
    {
        int first_half = BUFFER_SIZE - start_index;
        memcpy(temp_samples, &(ud->buffer)[start_index], first_half*sizeof(int16_t));
        memcpy(&temp_samples[first_half], ud->buffer, (SAMPLE_SIZE-first_half)*sizeof(int16_t)); 
    }

    return 0;
}

int create_samples(float* smoothed_samples, float* input_samples, int16_t* temp_samples)
{
    for (int i = 0; i < RECTANGLE_COUNT; i++) 
    {
        float sum = 0.0f;
        for (int j = 0; j < AVG_COUNT; j++) {
            int sample = temp_samples[i * AVG_COUNT + j];
            sum += fabs(sample / 32768.0f);
        }
        input_samples[i] = sum / AVG_COUNT;

        smoothed_samples[i] = smoothed_samples[i] + SMOOTHING_FACTOR * (input_samples[i] - smoothed_samples[i]);
    }

    return 0;
}

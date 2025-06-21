#ifndef AUDIO_PROCESSING_H
#define AUDIO_PROCESSING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#define BUFFER_SIZE 8192
#define SAMPLE_SIZE 1920
#define DEFAULT_RECTANGLE_COUNT 100
#define AVG_COUNT (SAMPLE_SIZE / RECTANGLE_COUNT)
#define AMPLIFY_POWER 500.0f
#define SMOOTHING_FACTOR 0.005f

extern int RECTANGLE_COUNT;

typedef struct userdata 
{
    Uint32 length;
    Uint8* position;
    int16_t buffer[BUFFER_SIZE];
    int idx;
} UserData;

typedef struct audiodata
{
    Uint32 length;
    Uint8* buffer;
    SDL_AudioSpec spec;
} AudioData;

void audio_callback(void* userdata, Uint8* stream, int len);
int load_file(char* file_path, UserData* ud, AudioData* ad);
int copy_audio_buffer(struct userdata* ud, int16_t* temp_samples);
int create_samples(float* smoothed_samples, float* input_samples, int16_t* temp_samples);

#endif
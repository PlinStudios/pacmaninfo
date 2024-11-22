#include <SDL2/SDL_mixer.h>

// Retorna 0 si no ocurre ningun error
int initMixer() {
    if (!Mix_Init(MIX_INIT_MP3)) {
        printf("Error initializing SDL_Mixer: %s\n", Mix_GetError());
        SDL_Quit();
        Mix_Quit();
        return 1;
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

    return 0;
}

Mix_Music* createMusic(char* filePath) {
    return Mix_LoadMUS(filePath);
}

Mix_Chunk* createSound(char* filePath) {
    return Mix_LoadWAV(filePath);
}

void playMusic(Mix_Music* music) {
    Mix_PlayMusic(music, -1);
}

void pauseMusic() {
    Mix_PauseMusic();
}

void continueMusic() {
    Mix_ResumeMusic();
}

void playSound(Mix_Chunk* sound) {
    Mix_PlayChannel(-1, sound, 0);
}

void freeMusic(Mix_Music* music) {
    Mix_FreeMusic(music);
}

void freeSound(Mix_Chunk* sound) {
    Mix_FreeChunk(sound);
}

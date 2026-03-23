#ifndef SDL_DISPLAY_H
#define SDL_DISPLAY_H

#include <cstdint>
#include <SDL2/SDL.h>

class SDLDisplay {
public:
    SDLDisplay();
    ~SDLDisplay();

    bool init(int scale = 4);
    bool processEvents();
    void clear();
    void present();
    void shutdown();

private:
    SDL_Window* window = nullptr;
    SDL_Event event;
    SDL_Renderer* renderer = nullptr;
    bool initialized = false;
};

#endif
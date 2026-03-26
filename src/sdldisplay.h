#ifndef SDL_DISPLAY_H
#define SDL_DISPLAY_H

#include <cstdint>
#include <SDL2/SDL.h>

class SDLDisplay {
public:
    SDLDisplay();
    ~SDLDisplay();

    bool init(int scale = 4);
    bool loop();
    void setFrameBuffer(const uint32_t* buffer);
    
private:
    SDL_Window* window = nullptr;
    SDL_Event event;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    int scale;
    bool initialized = false;
    bool processEvents();
    void shutdown();
    void present();
    void clear();
};

#endif
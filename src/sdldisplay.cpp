#include "sdldisplay.h"
#include <iostream>

static constexpr int GAMEBOY_WIDTH = 160;
static constexpr int GAMEBOY_HEIGHT = 144;

SDLDisplay::SDLDisplay() {}

SDLDisplay::~SDLDisplay() { shutdown(); }

bool SDLDisplay::init(int scale) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "Error initializing SDL: " << SDL_GetError() << '\n';
        return false;
    }

    window = SDL_CreateWindow("Gameboy", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAMEBOY_WIDTH*scale,
        GAMEBOY_HEIGHT*scale, SDL_WINDOW_SHOWN);

    if(!window){
        std::cout << "Error creating window: " << SDL_GetError()  << '\n';
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        std::cout << "Error creating renderer: " << SDL_GetError()  << '\n';
        return false;
    }

    texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        GAMEBOY_WIDTH,
        GAMEBOY_HEIGHT
    );
    if(!texture){
        std::cout << "Error creating renderer: " << SDL_GetError()  << '\n';
        return false;
    }

    initialized = true;
    return initialized;
}

bool SDLDisplay::processEvents() {
    while(SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_QUIT:
                return false;

            case SDL_KEYUP: 
            //TODO
                switch(event.key.keysym.sym){
                    case SDLK_w:
                        std::cout << "rilasciato w" << '\n';
                        break;
                    case SDLK_a:
                        std::cout << "rilasciato a" << '\n';
                        break;
                    case SDLK_s:
                        std::cout << "rilasciato s" << '\n';
                        break;
                    case SDLK_d:
                        std::cout << "rilasciato d" << '\n';
                        break;
                }
                break;

            case SDL_KEYDOWN: 
                switch(event.key.keysym.sym){
                    case SDLK_w:
                        std::cout << "premuto w" << '\n';
                        break;
                    case SDLK_a:
                        std::cout << "premuto a" << '\n';
                        break;
                    case SDLK_s:
                        std::cout << "premuto s" << '\n';
                        break;
                    case SDLK_d:
                        std::cout << "premuto d" << '\n';
                        break;
                }
                break;
        }
    }
    return true;
}

bool SDLDisplay::loop() {
    if(!processEvents()){
        return false;
    }
    clear();
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    present();
    return true;
}

void SDLDisplay::clear() {
    if(renderer){
        SDL_RenderClear(renderer);
    }
}

void SDLDisplay::present() {
    if(renderer){
        SDL_RenderPresent(renderer);
    }
}

void SDLDisplay::shutdown() {
    if (!initialized) return;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    window = nullptr;
    renderer = nullptr;
    texture = nullptr;
    initialized = false;
}

void SDLDisplay::setFrameBuffer(const uint32_t* buffer){
    SDL_UpdateTexture(texture, NULL, buffer, 160*sizeof(uint32_t));  
}
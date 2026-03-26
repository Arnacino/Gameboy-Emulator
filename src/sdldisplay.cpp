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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer){
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
    clear();
    bool shouldContinue = processEvents();
    present();
    return shouldContinue;
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
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    window = nullptr;
    renderer = nullptr;
    initialized = false;
}

void SDLDisplay::setFrameBuffer(const uint32_t* buffer){
    //qualcosa qualcosa texture qualcosa qualcosa
    processEvents();
}
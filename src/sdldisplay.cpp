#include "sdldisplay.h"
#include "joypad.h"
#include <iostream>

static constexpr int GAMEBOY_WIDTH = 160;
static constexpr int GAMEBOY_HEIGHT = 144;

SDLDisplay::SDLDisplay(Joypad* joypad) : joypad(joypad) {}

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
                std::cout << "Chiusura Finestra..." << '\n';
                return false;

            case SDL_KEYUP: 
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                        joypad->setKey(JoypadKey::Up, false);
                        break;
                    case SDLK_DOWN:
                        joypad->setKey(JoypadKey::Down, false);
                        break;
                    case SDLK_LEFT:
                        joypad->setKey(JoypadKey::Left, false);
                        break;
                    case SDLK_RIGHT:
                        joypad->setKey(JoypadKey::Right, false);
                        break;
                    case SDLK_z:
                        joypad->setKey(JoypadKey::A, false);
                        break;
                    case SDLK_x:
                        joypad->setKey(JoypadKey::B, false);
                        break;
                    case SDLK_TAB:
                        joypad->setKey(JoypadKey::Start, false);
                        break;
                    case SDLK_BACKSPACE:
                        joypad->setKey(JoypadKey::Select, false);
                        break;
                }
                break;

            case SDL_KEYDOWN: 
                if ((event.key.keysym.mod & KMOD_ALT) != 0 && event.key.keysym.sym == SDLK_F4) {
                    std::cout << "Chiusura richiesta con Alt+F4..." << '\n';
                    return false;
                }

                switch(event.key.keysym.sym){
                    case SDLK_UP:
                        joypad->setKey(JoypadKey::Up, true);
                        break;
                    case SDLK_DOWN:
                        joypad->setKey(JoypadKey::Down, true);
                        break;
                    case SDLK_LEFT:
                        joypad->setKey(JoypadKey::Left, true);
                        break;
                    case SDLK_RIGHT:
                        joypad->setKey(JoypadKey::Right, true);
                        break;
                    case SDLK_z:
                        joypad->setKey(JoypadKey::A, true);
                        break;
                    case SDLK_x:
                        joypad->setKey(JoypadKey::B, true);
                        break;
                    case SDLK_TAB:
                        joypad->setKey(JoypadKey::Start, true);
                        break;
                    case SDLK_BACKSPACE:
                        joypad->setKey(JoypadKey::Select, true);
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
    delete joypad;
}

void SDLDisplay::setFrameBuffer(const uint32_t* buffer){
    if (!texture) return;
    SDL_UpdateTexture(texture, NULL, buffer, 160*sizeof(uint32_t));  
}
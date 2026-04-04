#include "app.h"
#include "cpu.h"
#include "memory.h"
#include "registers.h"
#include "interrupt.h"
#include "instructions.h"
#include "ppu.h"
#include "sdldisplay.h"
#include "joypad.h"
#include <chrono>
#include <thread>
#include <thread>

static constexpr int FRAMETIME = 17556;

App::App(const char* filepath) {
    joypad = new Joypad();
    display = new SDLDisplay(joypad);
    memory = new Memory(filepath, joypad);
    registers = new Registers(); 
    interrupt = new Interrupt();
    instructions = new Instructions(registers, memory, interrupt);
    cpu = new CPU(memory, registers, instructions, interrupt);  
    ppu = new PPU(memory);
    }

App::~App(){
    delete display;
    delete memory;
    delete registers;
    delete interrupt;
    delete instructions;
    delete cpu;
    delete ppu;
    delete joypad;
}

void App::run(){
    int cycles = 0;
    if(!display->init()){
        return;
    }
    cpu->setRunning(true);
    bool shouldRun = true;

    using clock = std::chrono::steady_clock;
    const auto frameDuration = std::chrono::microseconds(16742);

    auto nextFrame = clock::now();

    while(shouldRun){
        nextFrame += frameDuration;

        //game logic
        int frameCycles = 0;
        while(frameCycles < FRAMETIME){
            cycles = cpu->step();
            frameCycles += cycles;
            ppu->update(cycles * 4);
        }
        display->setFrameBuffer(ppu->getFrameBuffer());
        shouldRun = display->loop();

        std::this_thread::sleep_until(nextFrame);
    }
}

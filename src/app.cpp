#include "app.h"
#include "cpu.h"
#include "memory.h"
#include "registers.h"
#include "interrupt.h"
#include "instructions.h"
#include "ppu.h"
#include "sdldisplay.h"

App::App(const char* filepath) {
    display = new SDLDisplay();
    memory = new Memory(filepath);
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
}

void App::run(){
    int cycles = 0;
    display->init();
    cpu->setRunning(true);
    bool shouldRun = true;
    while(shouldRun){
        cycles = cpu->step();
        ppu->update(cycles);
        //display->setFrameBuffer(ppu->getFramebuffer());
    }
}

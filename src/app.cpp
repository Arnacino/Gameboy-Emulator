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
    ppu = new PPU();
    }

App::~App(){}

void App::run(){
    display->init();
    while(true){
        cpu->setRunning(true);
        cpu->loop();
    }

}

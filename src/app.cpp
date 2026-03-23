#include "app.h"
#include "cpu.h"
#include "memory.h"
#include "registers.h"
#include "interrupt.h"
#include "instructions.h"

App::App(const char* filepath) {
    memory = new Memory(filepath); 
    registers = new Registers(); 
    interrupt = new Interrupt();
    instructions = new Instructions(registers, memory, interrupt);
    cpu = new CPU(memory, registers, instructions, interrupt);  
    }

App::~App(){}

void App::run(){
    cpu->run();
}

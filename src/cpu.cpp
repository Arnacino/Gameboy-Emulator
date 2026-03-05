#include "cpu.h"
#include "memory.h"
#include "registers.h"
#include "interrupt.h"
#include "instructions.h"

CPU::CPU(const char* filepath){
    memory = new Memory(filepath);
    registers = new Registers();
    interrupt = new Interrupt();
    instructions = new Instructions(registers, memory, interrupt);
    
    registers->pc = 0x0100;
    registers->sp = 0xFFFE;
    registers->af = 0x01B0;
    registers->bc = 0x0013;
    registers->de = 0x00D8;
    registers->hl = 0x014D;
}

int CPU::step(uint16_t pc){
    uint8_t nextInstr = memory->read(pc);
    registers->pc++;
    int cycles = instructions->execute(nextInstr);

    return cycles;
}

void CPU::loop(){
    bool running = true;

    while(running){
        
        int cycles = step(registers->pc);
        
        if(memory->read(0xFF02) == 0x81){
            char c = memory->read(0xFF01);
            std::cout << c;
            memory->write(0xFF02, 0x00);
        }
    }
}

void CPU::run(){
    loop();
}
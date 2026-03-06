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

int CPU::step(){
    uint8_t nextInstr = memory->read(registers->pc);
    registers->pc++;
    int cycles = instructions->execute(nextInstr);

    return cycles;
}

int CPU::handleInterrupt(uint8_t address, uint8_t bit){
    memory->write(0xFF0F, memory->read(0xFF0F) & bit);
    interrupt->disableIME(); 
    instructions->rst(address);
    return 5;
}

void CPU::loop(){
    bool running = true;
    int cycles = 0;

    while(running){
        cycles += step();

        interrupt->updateIME();

        if(interrupt->isIMEset()){
            uint8_t ifRegister = memory->read(0xFF0F); 
            uint8_t ieRegister = memory->read(0xFFFF); 

            if((ieRegister & ifRegister) & 0x1){
                cycles += handleInterrupt(0x40, 0xFE);

            }else if((ieRegister & ifRegister) & 0x2){
                cycles += handleInterrupt(0x48, 0xFD);

            }else if((ieRegister & ifRegister) & 0x4){
                cycles += handleInterrupt(0x50, 0xFB);

            }else if((ieRegister & ifRegister) & 0x8){
                cycles += handleInterrupt(0x58, 0xF7);

            }else if((ieRegister & ifRegister) & 0x10){
                cycles += handleInterrupt(0x60, 0xEF);
            }
        }
        
        //debug BLARRG
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
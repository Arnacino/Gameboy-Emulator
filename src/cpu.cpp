#include "cpu.h"
#include "memory.h"
#include "registers.h"
#include "interrupt.h"
#include "instructions.h"
#include <iostream>

CPU::CPU(Memory* memory, Registers* registers, Instructions* instructions, Interrupt* interrupt)
 : memory(memory), registers(registers), instructions(instructions), interrupt(interrupt)  {
    
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
    interrupt->setHalted(false);
    instructions->rst(address);
    return 5;
}

int CPU::handleTima(uint8_t tac, int timerCycles){

    int timesToIncrement = 0;

    if((tac & 0x03) == 0x00){
        if(timerCycles >= 256){
            timesToIncrement = timerCycles / 256;
            timerCycles = timerCycles % 256;
        }
    }else if((tac & 0x03) == 0x03){
        if(timerCycles >= 64){
            timesToIncrement = timerCycles / 64;
            timerCycles = timerCycles % 64;
        }
    }else if((tac & 0x03) == 0x02){
        if(timerCycles >= 16){
            timesToIncrement = timerCycles / 16;
            timerCycles = timerCycles % 16;
        }
    }else if((tac & 0x03) == 0x01){
        if(timerCycles >= 4){
            timesToIncrement = timerCycles / 4;
            timerCycles = timerCycles % 4;
        }
    }
    if(timesToIncrement > 0){
        uint16_t newTima = memory->read(0xFF05) + timesToIncrement;
        if(newTima > 0xFF){
            memory->write(0xFF05, memory->read(0xFF06));
            //interrupt
            memory->write(0xFF0F, memory->read(0xFF0F) | 0x4);
        }else{
            memory->write(0xFF05, newTima);
        }
    }

    return timerCycles;

}

void CPU::loop(){
    bool running = true;
    int cycles = 0;
    int temp = 0;
    int timerCycles = 0;

    while(running){
        if(!interrupt->isHalted()){
            temp = step();
            cycles += temp;
            timerCycles += temp;
        } else {
            cycles++;
            timerCycles++;
            if((memory->read(0xFF0F) & memory->read(0xFFFF) & 0x1F) != 0){
                interrupt->setHalted(false);
            }
        }

        interrupt->updateIME();

        if(interrupt->isIMEset()){
            uint8_t ifRegister = memory->read(0xFF0F); 
            uint8_t ieRegister = memory->read(0xFFFF); 

            if((ieRegister & ifRegister) & 0x1){
                temp = handleInterrupt(0x40, 0xFE);
                cycles += temp;
                timerCycles += temp;

            }else if((ieRegister & ifRegister) & 0x2){
                temp = handleInterrupt(0x48, 0xFD);
                cycles += temp;
                timerCycles += temp;

            }else if((ieRegister & ifRegister) & 0x4){
                temp = handleInterrupt(0x50, 0xFB);
                cycles += temp;
                timerCycles += temp;

            }else if((ieRegister & ifRegister) & 0x8){
                temp = handleInterrupt(0x58, 0xF7);
                cycles += temp;
                timerCycles += temp;

            }else if((ieRegister & ifRegister) & 0x10){
                temp = handleInterrupt(0x60, 0xEF);
                cycles += temp;
                timerCycles += temp;
            }
        }

        //TIMA
        uint8_t tac = memory->read(0xFF07);
        if(tac & 0x4){
            timerCycles = handleTima(tac, timerCycles);
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
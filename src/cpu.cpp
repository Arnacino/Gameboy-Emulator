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

//mCycles!
int CPU::executeCoreCycle(){
    if(!interrupt->isHalted()){
        uint8_t nextInstr = memory->read(registers->pc);
        registers->pc++;
        return instructions->execute(nextInstr);
    } else {
        if((memory->read(0xFF0F) & memory->read(0xFFFF) & 0x1F) != 0){
            interrupt->setHalted(false);
        }
        return 1;
    }
}

int CPU::handleInterrupt(){

    if(!interrupt->isIMEset()){
        return 0;
    }
    
    uint8_t bit = 0;
    uint16_t address = 0;
    
    uint8_t ifRegister = memory->read(0xFF0F); 
    uint8_t ieRegister = memory->read(0xFFFF); 
    uint8_t pending = (ifRegister & ieRegister) & 0x1F;
    
    if(pending == 0){
        return 0;
    }
    
    if(pending & 0x1){
        bit = 0xFE;
        address = 0x40;
    }else if(pending & 0x2){
        bit = 0xFD;
        address = 0x48;
    }else if(pending & 0x4){
        bit = 0xFB;
        address = 0x50;
    }else if(pending & 0x8){
        bit = 0xF7;
        address = 0x58;          
    }else if(pending & 0x10){
        bit = 0xEF;
        address = 0x60;              
    }else{
        return 0;
    }

    memory->write(0xFF0F, memory->read(0xFF0F) & bit);
    interrupt->disableIME(); 
    interrupt->setHalted(false);
    instructions->rst(address);
    return 5;
}

void CPU::handleTima(int cycles){
    uint8_t tac = memory->read(0xFF07);
    if((tac & 0x04) == 0){
        return;
    }
    
    int tCyles = cycles * 4;
    int period;

    if((tac & 0x03) == 0x03){
        period = 256;
    }else if((tac & 0x02) == 0x02){
        period = 64;
    }else if((tac & 0x01) == 0x01){
        period = 16;
    }else if((tac & 0x00) == 0){
        period = 1024;
    }

    timaCounter += tCyles;

    while(timaCounter >= period){
        timaCounter -= period;
        uint16_t tima = memory->read(0xFF05);
        if(tima >= 0xFF){
            //tima = tma
            memory->write(0xFF05, memory->read(0xFF06));
            //interrupt
            memory->write(0xFF0F, memory->read(0xFF0F) | 0x4);
        }else{
            tima++;
            memory->write(0xFF05, tima);
        }
    }
    
}

void CPU::handleDiv(int cycles){
    //tCycles
    memory->tickDiv(cycles*4);
}


int CPU::step(){
    int cycles = 0;
    
    if(!running){
        return 0;
    }

    cycles = executeCoreCycle();

    interrupt->updateIME();
    cycles += handleInterrupt();

    handleTima(cycles);
    handleDiv(cycles);

    debugPrints();

    return cycles;
}


void CPU::debugPrints(){
    //debug BLARRG
    if(memory->read(0xFF02) == 0x81){
        char c = memory->read(0xFF01);
        std::cout << c;
        memory->write(0xFF02, 0x00);
    }
}
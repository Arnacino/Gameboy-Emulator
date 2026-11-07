#include "cpu.h"
#include "../memory/memory.h"
#include "../memory/registers.h"
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
void CPU::executeInstructionCycle(){

    bool finished = instructions->execute(currentOpcode, instructionCycle);

    if(finished)
    {  
        instructionRunning = false;
        instructionCycle = 0;

        interrupt->updateIME();
    }
    else
    {
        instructionCycle++;
    }
}


bool CPU::interruptCanBeAccepted()
{
    uint8_t pending =
        memory->read(0xFF0F) &
        memory->read(0xFFFF) &
        0x1F;

    return interrupt->isIMEset() && pending != 0;
}



void CPU::tickTima(int cycles)
{
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
    }else if ((tac & 0x03) == 0){
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

void CPU::tickDiv(int cycles)
{
    //tCycles
    memory->tickDiv(cycles*4);
}

void CPU::tickDma(int cycles)
{
    if(memory->isDMAActive())
    {
        memory->DMATransfer(cycles);
    }
}

/*
    Il fetch dell'opcode principale è il primo M-cycle.
    Per le istruzioni più lunghe, instructionCycle indica i cicli
    successivi al fetch: cycle 1 corrisponde quindi a M2.
*/
void CPU::fetchInstruction()
{
    currentOpcode = memory->read(registers->pc);
    
    if(instructions->isHaltBugged())
    {
        instructions->setHaltBug(false);
    }
    else
    {
        ++registers->pc;
    }
    
    instructionCycle = 1;

    if (!instructions->isSingleMachineCycle(currentOpcode))
    {
        instructions->execute(currentOpcode, instructionCycle);
        instructionRunning = false;

        if (currentOpcode != 0xFB) {
            interrupt->updateIME();
        }   
    } 
    else 
    {
        instructionRunning = true;
    }
}

void CPU::executeHaltCycle()
{
    const uint8_t pending = (memory->read(0xFF0F) & memory->read(0xFFFF)) & 0x1F;
    
    if(pending == 0)
    {
        return;
    }

    interrupt->setHalted(false);

    if(interrupt->isIMEset())
    {
        interrupt->startInterrupt();
    }
}

void CPU::step()
{
    if (!running)
        return;

    if (interrupt->getInterruptPhase() != Interrupt::Phase::None) 
    {
        interrupt->cycle();
    }
    else if (instructionRunning) 
    {
        executeInstructionCycle();
    }
    else if (interrupt->isHalted()) 
    {
         executeHaltCycle();
    }
    else if (interruptCanBeAccepted()) 
    {
        interrupt->startInterrupt();
    }
    else 
    {
        fetchInstruction();
    }

    tickTima(1);
    tickDiv(1);
    tickDma(1);

    debugPrints();
}


void CPU::debugPrints(){
    //debug BLARRG
    if(memory->read(0xFF02) == 0x81){
        char c = memory->read(0xFF01);
        std::cout << c;
        memory->write(0xFF02, 0x00);
    }
}
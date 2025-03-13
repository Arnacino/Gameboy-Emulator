#ifndef CPU_H
#define CPU_H
#include "memory.h"
#include "registers.h"
#include "interrupt.h"
#include "instructions.h"

class CPU {
    private:
        Memory* memory;
        Registers* registers;
        Interrupt* interrupt;
        Instructions* instructions;
    
    public:
        CPU();
    };

#endif
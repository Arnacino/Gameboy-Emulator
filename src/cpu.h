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
        int step();
        void loop();
        int handleInterrupt(uint8_t address, uint8_t bit);
        bool running = false;
    public:
        CPU(const char* filepath);
        void run();
    };

#endif
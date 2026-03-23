#ifndef CPU_H
#define CPU_H
#include <cstdint>

class Memory;
class Registers;
class Interrupt;
class Instructions;

class CPU {
    private:
        Memory* memory;
        Registers* registers;
        Interrupt* interrupt;
        Instructions* instructions;
        int step();
        void loop();
        int handleInterrupt(uint8_t address, uint8_t bit);
        int handleTima(uint8_t tac, int timerCycles);
        bool running = false;
    public:
        CPU(Memory* memory, Registers* registers, Instructions* instructions, Interrupt* interrupt);
        void run();
    };

#endif
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
        int executeCoreCycle();
        int handleInterrupt();
        void handleTima(int cycles);
        bool running = false;
        int divCounter = 0;
        int timaCounter = 0;
        void debugPrints();
    public:
        CPU(Memory* memory, Registers* registers, Instructions* instructions, Interrupt* interrupt);
        int step();
        void setRunning(bool state){running = state;}
    };

#endif
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
        void executeInstructionCycle();
        void executeInterruptCycle();
        void executeHaltCycle();
        void startInterrupt();
        bool interruptCanBeAccepted();
        void fetchInstruction();
        void tickTima(int cycles);
        void tickDiv(int cycles);
        void tickDma(int cycles);
        void tickPpu(int cycles);
        void debugPrints();

        bool running = false;
        bool interruptRunning = false;
        bool instructionRunning = false;
        int timaCounter = 0;
        int instructionCycle = 0;
        uint8_t currentOpcode = 0;
    public:
        CPU(Memory* memory, Registers* registers, Instructions* instructions, Interrupt* interrupt);
        void step();
        void setRunning(bool state){running = state;}
    };

#endif
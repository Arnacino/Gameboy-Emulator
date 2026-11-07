#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "../memory/memory.h"
#include "../memory/registers.h"

class Interrupt
{

public:
    enum class Phase
    {
        None,
        Acknowledge,
        PushPcHigh,
        PushPcLow,
        Wait,
        JumpToVector
    };

private:
    bool IME = false;
    bool pendingIME = false;
    bool halted = false;
    Phase interruptPhase = Phase::None;
    Memory* memory;
    Registers* registers;
    uint16_t interruptVector = 0;
    uint8_t interruptBitMask = 0;

public:

    Interrupt(Memory* memory, Registers* registers);
    Phase getInterruptPhase();
    void enableIME();
    void setHalted(bool condition);
    void startInterrupt();
    bool isHalted();
    bool isIMEset();
    void disableIME();
    void scheduleEnableIME();
    void updateIME();
    void cycle();

};


#endif
#include "interrupt.h"

    Interrupt::Interrupt(Memory* memory, Registers* registers): 
        memory(memory), registers(registers){};

    void Interrupt::cycle()
    {
        switch (interruptPhase)
        {
            case Interrupt::Phase::Acknowledge:
                memory->write(0xFF0F, memory->read(0xFF0F) & interruptBitMask);
                disableIME();
                setHalted(false);
                interruptPhase = Interrupt::Phase::Wait;
                break;

            case Interrupt::Phase::Wait:
                interruptPhase = Interrupt::Phase::PushPcHigh;
                break;

            case Interrupt::Phase::PushPcHigh:
                registers->sp--;
                memory->write(registers->sp, (registers->pc >> 8) & 0xFF);
                interruptPhase = Interrupt::Phase::PushPcLow;
                break;

            case Interrupt::Phase::PushPcLow:
                registers->sp--;
                memory->write(registers->sp, registers->pc & 0xFF);
                interruptPhase = Interrupt::Phase::JumpToVector;
                break;

            case Interrupt::Phase::JumpToVector:
                registers->pc = interruptVector;
                interruptPhase = Interrupt::Phase::None;
                break;

            default:
                break;
        }
    }
    
    void Interrupt::updateIME()
    {
        if(this->pendingIME)
        {
            this->IME = true;
            this->pendingIME = false;
        }
    }

    void Interrupt::disableIME()
    {
        this->IME = false;
        this->pendingIME = false;
    }

    void Interrupt::scheduleEnableIME()
    {
        this->pendingIME = true;
    }

    Interrupt::Phase Interrupt::getInterruptPhase()
    {
        return interruptPhase;
    }

    void Interrupt::enableIME()
    {
        this->IME = true;
    }

    void Interrupt::setHalted(bool condition)
    {
        halted = condition;
    }

    void Interrupt::startInterrupt()
    {
        uint8_t ifRegister = memory->read(0xFF0F); 
        uint8_t ieRegister = memory->read(0xFFFF); 
        uint8_t pending = ifRegister & ieRegister & 0x1F;
        if (pending & 0x01) {
            interruptVector = 0x0040;
            interruptBitMask = 0xFE;
        }
        else if (pending & 0x02) {
            interruptVector = 0x0048;
            interruptBitMask = 0xFD;
        }
        else if (pending & 0x04) {
            interruptVector = 0x0050;
            interruptBitMask = 0xFB;
        }
        else if (pending & 0x08) {
            interruptVector = 0x0058;
            interruptBitMask = 0xF7;
        }
        else if (pending & 0x10) {
            interruptVector = 0x0060;
            interruptBitMask = 0xEF;
        }

        interruptPhase = Phase::Acknowledge;
    }

    bool Interrupt::isHalted()
    {
        return halted;
    }

    bool Interrupt::isIMEset()
    {
        return this->IME;
    }
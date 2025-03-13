#include "cpu.h"
#include "memory.h"
#include "registers.h"
#include "interrupt.h"
#include "instructions.h"

CPU::CPU(){
    memory = new Memory();
    registers = new Registers();
    interrupt = new Interrupt();
    instructions = new Instructions(registers, memory, interrupt);
}
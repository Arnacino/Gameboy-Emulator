#include <iostream>
#include <string.h>
#include "cpu.h"

int main(int, char**){
    
    //char filepath[] = "C:\\\\Users\\\\samue\\\\Desktop\\\\Progetti\\\\gb-test-roms\\\\cpu_instrs\\\\individual\\\\02-interrupts.gb";
    char filepath[] = "C:\\Users\\samue\\Desktop\\Progetti\\gb-test-roms\\instr_timing\\instr_timing.gb";
    CPU* cpu = new CPU(filepath);
    cpu->run();
}

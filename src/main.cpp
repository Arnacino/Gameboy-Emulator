#include <iostream>
#include <string.h>
#include "cpu.h"

int main(int, char**){
    char filepath[] = "C:\\Users\\samue\\Desktop\\Progetti\\gb-test-roms\\cpu_instrs\\individual\\11-op a,(hl).gb";
    CPU* cpu = new CPU(filepath);
    cpu->run();
}

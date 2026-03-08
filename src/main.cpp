#include <iostream>
#include <string.h>
#include "cpu.h"

int main(int, char**){
    
    char filepath[] = "C:\\Users\\samue\\Desktop\\Progetti\\gb-test-roms\\dmg_sound\\rom_singles\\01-registers.gb";
    CPU* cpu = new CPU(filepath);
    cpu->run();
}

#include <iostream>
#include <string.h>
#include "cpu.h"

int main(int, char**){
    char filepath[] = "C:\\Users\\samue\\Desktop\\Progetti\\cpu_instrs.gb";
    CPU* cpu = new CPU(filepath);
    cpu->run();
}

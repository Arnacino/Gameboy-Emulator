#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <iostream>

class Memory
{
    public: 
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t content);
    bool loadRom(const char* filepath);

    Memory(const char* filepath){
        if(!loadRom(filepath)){
            std::cout << "ROM is not valid or path is wrong" << std::endl;
        }else{
            std::cout<< "ROM loaded successfully" << std::endl;
        }
    }

    private:
    uint8_t rom00[0x4000] = {}; //fixed ROM bank 00
    uint8_t rom01[0x4000] = {}; //not fixed ROM bank 01-nn
    uint8_t vram[0x2000] = {};
    uint8_t extRam[0x2000] = {}; 
    uint8_t wRam00[0x1000] = {};
    uint8_t wRam01[0x1000] = {}; //qualcosa CGB da capire
    uint8_t echoRam[0x1E00] = {};
    uint8_t IORegisters[0x100] = {};
    uint8_t hRam[0x80] = {};
    uint8_t oam[0x100] = {}; //Object Attribute Memory. Contiene gli attributi degli oggetti mostrati a schermo
    uint8_t interruptEnableRegister = 0;
    
};

#endif
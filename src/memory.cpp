#include "Memory.h"

void Memory::write(uint16_t address, uint8_t value){

     //rom 00
    if(address >= 0x0000 && address <= 0x3FFF){
        rom00[address] = value;
    }

    //rom 01
    if(address >= 0x4000 && address <= 0x7FFF){
        rom01[address%0x4000] = value;
    }

    //vram
    if(address >= 0x8000 && address <= 0x9FFF){
        vram[address%0x2000] = value;
    }

    //external ram
    if(address >=0xA000 && address <= 0xBFFF){
        extRam[address%0x2000] = value;
    }

    //work ram 
    if(address >= 0xC000 && address <= 0xCFFF){
        wRam00[address%0x1000] = value;
        echoRam[address%0x1E00] = value;
    }

    //work ram CGB QUALCOSA
    if(address >= 0xD000 && address <= 0xDFFF){
        wRam01[address%0x1000] = value;
        if(address <= 0xDDFF){
            echoRam[address%0x1E00] = value;
        }
    }

    //object attribute memory
    if(address >= 0xFE00 && address <= 0xFE9F){
        oam[address%0x100] = value;
    }

    //ioregisters
    if(address >= 0xFF00 && address <= 0xFF7F){
        IORegisters[address%0x100] = value;
    }

    //high ram
    if(address >=0xFF80 && address <= 0xFFFE){
        hRam[address%0x80] = value;
    }
}

uint8_t Memory::read(uint16_t address){

    //rom 00
    if(address >= 0x0000 && address <= 0x3FFF){
        return rom00[address];
    }

    //rom 01
    if(address >= 0x4000 && address <= 0x7FFF){
        return rom01[address%0x4000];
    }

    //vram
    if(address >= 0x8000 && address <= 0x9FFF){
        return vram[address%0x2000];
    }

    //external ram
    if(address >=0xA000 && address <= 0xBFFF){
        return extRam[address%0x2000];
    }

    //work ram 
    if(address >= 0xC000 && address <= 0xCFFF){
        return wRam00[address%0x1000];
    }

    //work ram CGB QUALCOSA
    if(address >= 0xD000 && address <= 0xDFFF){
        return wRam01[address%0x1000];
    }

    //echo ram
    if(address >= 0xF000 && address <= 0xFDFF){
        return echoRam[address%0x1E00];
    }

    //object attribute memory
    if(address >= 0xFE00 && address <= 0xFE9F){
        return oam[address%0x100];
    }

    //ioregisters
    if(address >= 0xFF00 && address <= 0xFF7F){
        return IORegisters[address%0x100];
    }

    //high ram
    if(address >=0xFF80 && address <= 0xFFFE){
        return hRam[address%0x80];
    }

    
}
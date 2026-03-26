#include "ppu.h"
#include "memory.h"

PPU::PPU(Memory* memory) :  memory(memory){}

PPU::~PPU(){}


uint8_t PPU::getTileDataByte(uint16_t address){
    return memory->read(address);
}

void PPU::renderTile(){
    uint32_t tile;
    for(int i = 0; i < 8; ++i){
        
    }
}

void PPU::update(int cycles){
    //gestione del timing eccetera eccetera
}

void PPU::renderScanline(){

}
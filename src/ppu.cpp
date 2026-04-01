#include "ppu.h"
#include "memory.h"
#include "ppu_mode.h"
#include <algorithm>

static constexpr int GAMEBOY_WIDTH = 160;
static constexpr int GAMEBOY_HEIGHT = 144;

PPU::PPU(Memory* memory) :  memory(memory){
    framebuffer = new uint32_t[160 * 144];
}

PPU::~PPU(){
    delete[] framebuffer;
}


uint16_t PPU::getTileDataByte(uint16_t address){
    return ((memory->rawRead(address) << 8) | memory->rawRead(address + 0x01));
}

//renderizza 8 pixel di un tile
void PPU::renderTileRow(int x, int y, uint16_t address){
    uint16_t tileMemory = getTileDataByte(address);
    //prendo i pixel singoli da tileMemory
    for(int i = 0; i < 8; ++i){
        uint8_t bit0 = (tileMemory >> (7 - i)) & 1;
        uint8_t bit1 = (tileMemory >> (15 - i)) & 1;
        uint8_t colorIndex = (bit1 << 1) | bit0;
        framebuffer[y * GAMEBOY_WIDTH + x] = palette[colorIndex];
        ++x;
    }
}

void PPU::update(int dotCycles){
    cycleCount += dotCycles;
    while(true){
        switch (currentMode){
            case PPUMode::OamScan: {
                if(cycleCount < 80) return;
                cycleCount -= 80;
                currentMode = PPUMode::Drawing;
                memory->setPPUMode(currentMode);
                break;
            }

            case PPUMode::Drawing: {
                if(cycleCount < 172) return;
                cycleCount -= 172;
                renderScanline();
                currentMode = PPUMode::HBlank;
                memory->setPPUMode(currentMode);
                break;
            }
               
            case PPUMode::HBlank: {
                if(cycleCount < 204) return;
                cycleCount -= 204;
                uint8_t ly = memory->rawRead(0xFF44);
                ly++;
                memory->rawWrite(0xFF44, ly);
                if(ly == 144){
                    currentMode = PPUMode::VBlank;
                    memory->setPPUMode(currentMode);
                    memory->rawWrite(0xFF0F, memory->rawRead(0xFF0F) | 0x01);
                }else{
                    currentMode = PPUMode::OamScan;
                    memory->setPPUMode(currentMode);
                }
                break;
            }
               
            case PPUMode::VBlank: {
                if(cycleCount < 456) return;
                cycleCount -= 456;
                uint8_t ly = memory->rawRead(0xFF44);
                ly++;

                if(ly > 153){
                    ly = 0;
                    currentMode = PPUMode::OamScan;
                    memory->setPPUMode(currentMode);

                }
                memory->rawWrite(0xFF44, ly);
                break;
            }

            default:
                break;
        }
    }
}

//renderizza una riga DI PIXEL!!
void PPU::renderScanline(){
    uint8_t ly = memory->rawRead(0xFF44);
    //ly rappresenta a quale y dello schermo sono, 
    //faccio diviso 8 per capire a quale tile corrisponde
    int tileRow = ly / 8;

    //questo rappresenta quale riga sto usando all'interno della tile
    //essendo essa composta di 8 righe di pixel
    int rowInsideTile = ly % 8;
    
    for(int tileCol = 0; tileCol < 20; ++tileCol){
        //gli indirizzi a partire da 0x9800 sono gli indirizzi che mi dicono quale tile
        //devo usare. Il valore che ottengo leggendo questo indirizzo si usa per trovare
        //l'address finale dei tile.
        uint16_t mapAddr = 0x9800 + tileRow * 32 + tileCol;
        uint8_t tileID = memory->rawRead(mapAddr);
        //calcolo l'indirizzo finale del tile grazie all'ID ottenuto prima
        uint16_t address = 0x8000 + tileID * 16 + rowInsideTile * 2;

        renderTileRow(tileCol * 8, ly, address);
    }

}
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


void PPU::updateStatReg(){
    uint8_t stat = memory->rawRead(0xFF41);
    const uint8_t ly = memory->rawRead(0xFF44);
    const uint8_t lyc = memory->rawRead(0xFF45);

    stat = static_cast<uint8_t>((stat & 0xFC) | static_cast<uint8_t>(currentMode));

    const bool lycMatch = (ly == lyc);
    if (lycMatch) {
        stat |= 0x04;
    } else {
        stat &= static_cast<uint8_t>(~0x04);
    }

    memory->rawWrite(0xFF41, stat);
    // Build STAT interrupt line from enabled sources
    const bool mode0Req = (currentMode == PPUMode::HBlank)  && ((stat & 0x08) != 0); // bit 3
    const bool mode1Req = (currentMode == PPUMode::VBlank)  && ((stat & 0x10) != 0); // bit 4
    const bool mode2Req = (currentMode == PPUMode::OamScan) && ((stat & 0x20) != 0); // bit 5
    const bool lycReq   = lycMatch                          && ((stat & 0x40) != 0); // bit 6

    const bool statIrqLine = mode0Req || mode1Req || mode2Req || lycReq;

    // Rising edge detect: request LCD STAT interrupt (IF bit 1)
    if (!prevStatIrqLine && statIrqLine) {
        memory->rawWrite(0xFF0F, static_cast<uint8_t>(memory->rawRead(0xFF0F) | 0x02));
    }

    prevStatIrqLine = statIrqLine;
}

void PPU::update(int dotCycles){
    
    uint8_t lcdc = memory->rawRead(0xFF40);
    
    if((lcdc & 0x80) == 0){
        cycleCount = 0;
        currentMode = PPUMode::HBlank;
        memory->setPPUMode(currentMode);
        memory->rawWrite(0xFF44, 0); 
        updateStatReg();
        return;
    }
    
    cycleCount += dotCycles;
    while(true){
        switch (currentMode){
            case PPUMode::OamScan: {
                if(cycleCount < 80) return;
                cycleCount -= 80;
                currentMode = PPUMode::Drawing;
                memory->setPPUMode(currentMode);
                updateStatReg();
                break;
            }

            case PPUMode::Drawing: {
                if(cycleCount < 172) return;
                cycleCount -= 172;
                renderScanline();
                currentMode = PPUMode::HBlank;
                memory->setPPUMode(currentMode);
                updateStatReg();
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
                    memory->rawWrite(0xFF0F, memory->rawRead(0xFF0F) | 0x01); //interrupt vblank
                }else{
                    currentMode = PPUMode::OamScan;
                    memory->setPPUMode(currentMode);
                }
                updateStatReg();
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
                updateStatReg();
                break;
            }
        }
    }
}



void PPU::renderScanline(){
    uint8_t lcdc = memory->rawRead(0xFF40);
    uint8_t ly = memory->rawRead(0xFF44);

    // riempi la scanline con colore 0
    if ((lcdc & 0x01) == 0) {
        for (int x = 0; x < 160; ++x) {
            framebuffer[ly * GAMEBOY_WIDTH + x] = palette[0];
        }
        return;
    }

    uint16_t bgMapBase = (lcdc & 0x08) ? 0x9C00 : 0x9800;
    bool unsignedTiles = (lcdc & 0x10) != 0;

    uint8_t scx = memory->rawRead(0xFF43);
    uint8_t scy = memory->rawRead(0xFF42);

    for (int x = 0; x < 160; ++x) {
        uint8_t bgX = static_cast<uint8_t>(x + scx);
        uint8_t bgY = static_cast<uint8_t>(ly + scy);

        uint8_t tileCol = bgX / 8;
        uint8_t tileRow = bgY / 8;
        uint8_t inTileX = bgX % 8;
        uint8_t inTileY = bgY % 8;
        
        uint16_t mapAddr = bgMapBase + tileRow * 32 + tileCol;
        uint8_t tileID = memory->rawRead(mapAddr);


        uint16_t tileRowAddr;
        if(unsignedTiles){
            tileRowAddr = 0x8000 + tileID * 16 + inTileY * 2;
        }else{
            int8_t signedID = static_cast<int8_t>(tileID);
            tileRowAddr = 0x9000 + signedID * 16 + inTileY * 2;
        }

        uint8_t lowByte = memory->rawRead(tileRowAddr);
        uint8_t highByte = memory->rawRead(tileRowAddr + 1);
        
        int bit = 7 - inTileX;
        uint8_t bit0 = (lowByte >> bit) & 1;
        uint8_t bit1 = (highByte >> bit) & 1;
        uint8_t colorIndex = static_cast<uint8_t>((bit1 << 1) | bit0);

        framebuffer[ly * GAMEBOY_WIDTH + x] = palette[colorIndex];

    }

}
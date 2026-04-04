#include "ppu.h"
#include "memory.h"
#include "ppu_mode.h"
#include <algorithm>

static constexpr int GAMEBOY_WIDTH = 160;
static constexpr int GAMEBOY_HEIGHT = 144;

PPU::PPU(Memory* memory) :  memory(memory){
    frameBuffer = new uint32_t[160 * 144];
    OBJFrameBuffer = new uint32_t[160 * 144];
    BGFrameBuffer = new uint32_t[160 * 144];
    winFrameBuffer = new uint32_t[160 * 144];
}

PPU::~PPU(){
    delete[] frameBuffer;
    delete[] OBJFrameBuffer;
    delete[] BGFrameBuffer;
    delete[] winFrameBuffer;
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
    
    //LCDC bit 7, ppu disabilitata
    if((lcdc & 0x80) == 0){
        cycleCount = 0;
        currentMode = PPUMode::HBlank;
        memory->setPPUMode(currentMode);
        memory->rawWrite(0xFF44, 0); 
        updateStatReg();
        return;
    }
    
    //dotCycles è già in DOT. Quindi 1 dotCycle = 1 DOT
    cycleCount += dotCycles;
    while(true){
        switch (currentMode){
            case PPUMode::OamScan: {
                if(cycleCount < 80) return;
                cycleCount -= 80;
                currentMode = PPUMode::Drawing;
                memory->setPPUMode(currentMode);
                updateStatReg();
                pipelineDelay = 12 + (memory->rawRead(0xFF43) & 0x07); //delay di SCX % 8
                lx = 0;
                findSprites();
                break;
            }

            case PPUMode::Drawing: {
                if(cycleCount <= 0) return;
                if(pipelineDelay > 0){
                    --pipelineDelay;
                    --cycleCount;
                    break;
                }
                renderPixel();
                ++lx;
                --cycleCount;
                if(lx == 160){
                    currentMode = PPUMode::HBlank;
                    memory->setPPUMode(currentMode);
                    updateStatReg();
                }
                break;
            }
               
            case PPUMode::HBlank: {
                if(cycleCount < 204) return;
                cycleCount -= 204;
                uint8_t ly = memory->rawRead(0xFF44);
                ++ly;
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
                ++ly;

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

void PPU::findSprites(){

    spriteCount = 0;
    sprites.fill(SpriteCandidate{});

    uint8_t lcdc = memory->rawRead(0xFF40);
    uint8_t ly = memory->rawRead(0xFF44);
    int spriteHeight = (lcdc & 0x04) ? 16 : 8;

    for(uint8_t s = 0; s < 40; ++s){
        
        if(spriteCount >= 10) return;

        uint16_t spriteAddr = 0xFE00 + s * 4;
        uint8_t yPos = memory->rawRead(spriteAddr);
        uint8_t xPos = memory->rawRead(spriteAddr + 1);
        uint8_t tileIndex = memory->rawRead(spriteAddr + 2);
        uint8_t flags = memory->rawRead(spriteAddr + 3);
        
        int spriteTop = static_cast<int>(yPos) - 16;
        if(ly >= spriteTop && ly < spriteTop + spriteHeight){
            sprites[spriteCount] = {
                s,
                xPos,
                yPos,
                tileIndex,
                flags,
            };
            ++spriteCount;
        }
    }

}

void PPU::renderPixel(){
    const uint8_t lcdc = memory->rawRead(0xFF40);
    const uint8_t ly = memory->rawRead(0xFF44);
    
    uint8_t scx = memory->rawRead(0xFF43);
    uint8_t scy = memory->rawRead(0xFF42);

    renderBGPixel(lcdc, ly, scx, scy);
    renderOBJPixel(lcdc, ly);

    //aggiungi penalità di 6 dot
    renderWinPixel(lcdc, ly);

    mergeBuffers(ly);

}

void PPU::renderOBJPixel(uint8_t lcdc, uint8_t ly){

    const int index = ly * GAMEBOY_WIDTH + lx;
    OBJFrameBuffer[index] = 0x00000000;

    //LCDC bit 1, se spento obj disabilitati
    if((lcdc & 0x02) == 0) return;

    int spriteHeight = (lcdc & 0x04) ? 16 : 8;

    for(int i = 0; i < spriteCount; ++i){
        int spriteTop = static_cast<int>(sprites[i].oamY) - 16;
        int spriteLeft = static_cast<int>(sprites[i].oamX) - 8;
        uint16_t tileAddr = 0;

        if(ly >= spriteTop && ly < spriteTop + spriteHeight && 
        lx >= spriteLeft && lx < spriteLeft + 8){
            
            int rowInObj = ly - spriteTop;
            
            if(sprites[i].attributes & 0x40){
                rowInObj = spriteHeight - 1 - rowInObj;
            }

            if(spriteHeight == 8){
                tileAddr = 0x8000 + (sprites[i].tileID * 16) + (rowInObj * 2);
            }else{
                uint8_t baseTile = sprites[i].tileID & 0xFE;   // forza tile pari
                uint8_t tileOffset = (rowInObj >= 8) ? 1 : 0;   // top/bottom tile
                uint8_t rowInTile = rowInObj & 0x07;            // modulo 8
                tileAddr = 0x8000 + ((baseTile + tileOffset) * 16) + (rowInTile * 2);
            }

            uint8_t lowByte = memory->rawRead(tileAddr);
            uint8_t highByte = memory->rawRead(tileAddr + 1);

            int xInObj = lx - spriteLeft;

            if(sprites[i].attributes & 0x20){
                xInObj = 7 - xInObj;
            }

            int bit = 7 - xInObj;
    
            uint8_t bit0 = (lowByte >> bit) & 1;
            uint8_t bit1 = (highByte >> bit) & 1;

            uint8_t colorIndex = static_cast<uint8_t>((bit1 << 1) | bit0);

            // trasparente: lascia visibile il BG
            if(colorIndex == 0) continue;

            uint8_t obp = (sprites[i].attributes & 0x10) ? 
                memory->rawRead(0xFF49) : memory->rawRead(0xFF48);

            uint8_t paletteIndex = (obp >> (colorIndex * 2)) & 0x03;

            OBJFrameBuffer[index] = palette[paletteIndex];
            break;
        }
    }
}

void PPU::renderBGPixel(uint8_t lcdc, uint8_t ly, uint8_t scx, uint8_t scy){

    const int index = ly * GAMEBOY_WIDTH + lx;

    // LCDC bit 0, BG bianco
    if ((lcdc & 0x01) == 0) {
        BGFrameBuffer[index] = palette[0];
        return;
    }

    uint8_t bgX = static_cast<uint8_t>(lx + scx);
    uint8_t bgY = static_cast<uint8_t>(ly + scy);

    //LCDC bit 3
    uint16_t bgMapBase = (lcdc & 0x08) ? 0x9C00 : 0x9800;
    
    //LCDC bit 4
    bool unsignedTiles = (lcdc & 0x10) != 0;

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
    uint8_t bgp = memory->rawRead(0xFF47);
    uint8_t paletteIndex = (bgp >> (colorIndex * 2)) & 0x03;

    BGFrameBuffer[index] = palette[paletteIndex];
}

void PPU::mergeBuffers(uint8_t ly){

    const int index = ly * GAMEBOY_WIDTH + lx;
    frameBuffer[index] = BGFrameBuffer[index];

    if (winFrameBuffer[index] != 0x00000000) {
        frameBuffer[index] = winFrameBuffer[index];
    }

    if(OBJFrameBuffer[index] != 0x00000000){
        frameBuffer[index] = OBJFrameBuffer[index];
    }
}

void PPU::renderWinPixel(uint8_t lcdc, uint8_t ly){

    const int index = ly * GAMEBOY_WIDTH + lx;
    
    winFrameBuffer[index] = 0x00000000;
    if ((lcdc & 0x20) == 0) return; 

    uint8_t wx = memory->rawRead(0xFF4B);
    uint8_t wy = memory->rawRead(0xFF4A);


    if (lx + 7 < wx || ly < wy) {
        // Fuori dal window, il background rimarrà visibile
        return;
    }
    
    uint16_t tileMapAddr = (lcdc & 0x40) ? 0x9C00 : 0x9800;

    uint8_t winX = lx + 7 - wx;
    uint8_t winY = ly - wy;

    //LCDC bit 4
    bool unsignedTiles = (lcdc & 0x10) != 0;

    uint8_t tileCol = winX / 8;
    uint8_t tileRow = winY / 8;
    uint8_t inTileX = winX % 8;
    uint8_t inTileY = winY % 8;
    
    uint16_t mapAddr = tileMapAddr + tileRow * 32 + tileCol;
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
    uint8_t bgp = memory->rawRead(0xFF47);
    uint8_t paletteIndex = (bgp >> (colorIndex * 2)) & 0x03;

    winFrameBuffer[index] = palette[paletteIndex];
}
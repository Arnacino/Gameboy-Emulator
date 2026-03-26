#ifndef PPU_H
#define PPU_H
#include <cstdint>

class Memory;

class PPU
{
private:
    Memory* memory;
    uint32_t* framebuffer;
    int cycleCount = 0;
    static constexpr uint32_t palette[4] = {
        0xFFFFFFFF,  // color index 0 = bianco
        0xFFAAAAAA,  // color index 1 = grigio chiaro
        0xFF555555,  // color index 2 = grigio scuro
        0xFF000000   // color index 3 = nero
    };
    
    uint16_t getTileDataByte(uint16_t address);
    void renderTileRow(int x, int y, uint16_t address);
    void renderScanline();

    public:
    PPU(Memory* memory);
    ~PPU();
    
    void update(int cycles);
    const uint32_t* getFramebuffer() const { return framebuffer; }
};

#endif
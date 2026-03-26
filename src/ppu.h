#ifndef PPU_H
#define PPU_H
#include <cstdint>

class Memory;

class PPU
{
private:
    Memory* memory;
    uint32_t* framebuffer;
    uint8_t getTileDataByte(uint16_t address);
    void renderTile();
    void renderScanline();
    
    public:
    PPU(Memory* memory);
    ~PPU();
    void update(int cycles);
    const uint32_t* getFramebuffer() const { return framebuffer; }
};

#endif
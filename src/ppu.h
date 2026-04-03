#ifndef PPU_H
#define PPU_H
#include <cstdint>

class Memory;

enum class PPUMode : uint8_t;

class PPU
{
private:
    Memory* memory;
    uint32_t* framebuffer;
    int cycleCount = 0;
    bool prevStatIrqLine = false;
    PPUMode currentMode = static_cast<PPUMode>(2); // 2 = OAMSCAN
    static constexpr uint32_t palette[4] = {
        0xFF9a9e3f,  // color index 0 = bianco
        0xFF496b22,  // color index 1 = grigio chiaro
        0xFF0e450b,  // color index 2 = grigio scuro
        0xFF1b2a09   // color index 3 = nero
    };
    
    uint16_t getTileDataByte(uint16_t address);
    void renderTileRow(int x, int y, uint16_t address);
    void renderScanline();
    void updateStatReg();

    public:
    PPU(Memory* memory);
    ~PPU();
    
    void update(int dotCycles);
    const uint32_t* getFramebuffer() const { return framebuffer; }
};

#endif
#ifndef PPU_H
#define PPU_H
#include <array>
#include <cstdint>

class Memory;

enum class PPUMode : uint8_t;



class PPU
{
private:
    Memory* memory;
    uint32_t* framebuffer;
    uint32_t* BGframebuffer;
    uint32_t* OBJframebuffer;
    int cycleCount = 0;
    int pipelineDelay = 12;
    int lx = 0;
    int spriteCount = 0;
    bool prevStatIrqLine = false;
    struct SpriteCandidate {
        uint8_t oamIndex;
        uint8_t oamX;
        uint8_t oamY;
        uint8_t tileID;
        uint8_t attributes;
    };

    std::array<SpriteCandidate, 10> sprites{};
    PPUMode currentMode = static_cast<PPUMode>(2); // 2 = OAMSCAN
    static constexpr uint32_t palette[4] = {
        0xFF9a9e3f,  // color index 0 = bianco
        0xFF496b22,  // color index 1 = grigio chiaro
        0xFF0e450b,  // color index 2 = grigio scuro
        0xFF1b2a09   // color index 3 = nero
    };
    
    void updateStatReg();
    void findSprites();
    void renderPixel();
    void renderBGPixel(uint8_t lcdc, uint8_t ly, uint8_t scx, uint8_t scy);
    void renderOBJPixel(uint8_t lcdc, uint8_t ly);
    void mergeBuffers(uint8_t ly);

    public:
    PPU(Memory* memory);
    ~PPU();
    
    void update(int dotCycles);
    const uint32_t* getFramebuffer() const { return framebuffer; }
};

#endif
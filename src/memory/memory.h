#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <iostream>

enum class PPUMode : uint8_t;

enum class JoypadKey : uint8_t;

class Joypad;

class Memory
{
    public: 
    uint8_t read(uint16_t address);
    uint8_t rawRead(uint16_t address);
    void write(uint16_t address, uint8_t content);
    void rawWrite(uint16_t address, uint8_t content);
    bool loadRom(const char* filepath);
    void tickDiv(int tCycles);
    void setPPUMode(PPUMode mode);
    bool isDMAActive() { return DMAActive; }
    void DMATransfer(int cycles);
    void setJoypadKey(JoypadKey key, bool pressed);
    
    Memory(const char* filepath, Joypad* joypad) : joypad(joypad){
        if(!loadRom(filepath)){
            std::cout << "ROM is not valid or path is wrong" << std::endl;
        }else{
            std::cout<< "ROM loaded successfully" << std::endl;
        }

        rawWrite(0xFF00, 0xCF);
        rawWrite(0xFF40, 0x91); //LCDC
        rawWrite(0xFF41, 0x85); //STAT
        rawWrite(0xFF42, 0x00);
        rawWrite(0xFF43, 0x00);
        rawWrite(0xFF44, 0x00);
        rawWrite(0xFF45, 0x00);
        rawWrite(0xFF47, 0xFC); //BGP
        rawWrite(0xFF4A, 0x00); //WY
        rawWrite(0xFF4B, 0x00); //WX
        rawWrite(0xFF0F, 0xE1); //IF
        rawWrite(0xFFFF, 0x00); //IE
        
    }
    
    ~Memory(){
    }
    
    private:
    int divCounter = 0;
    PPUMode mode = static_cast<PPUMode>(0);
    uint8_t rom00[0x4000] = {}; //fixed ROM bank 00
    uint8_t rom01[0x4000] = {}; //not fixed ROM bank 01-nn
    uint8_t vram[0x2000] = {};
    uint8_t extRam[0x2000] = {}; 
    uint8_t wRam00[0x1000] = {};
    uint8_t wRam01[0x1000] = {}; 
    uint8_t echoRam[0x1E00] = {};
    uint8_t IORegisters[0x100] = {};
    uint8_t hRam[0x80] = {};
    uint8_t oam[0x100] = {}; //Object Attribute Memory. Contiene gli attributi degli oggetti mostrati a schermo
    uint8_t interruptEnableRegister = 0;
    bool DMAActive = false;
    int DMATicksLeft = 0;
    uint16_t DMASource = 0;
    uint16_t DMAIndex = 0;
    Joypad* joypad;

    void incrementDiv();
    void LYCCompare();
    uint8_t readRaw(uint16_t address);
    void writeRaw(uint16_t address, uint8_t value);
    
};

#endif
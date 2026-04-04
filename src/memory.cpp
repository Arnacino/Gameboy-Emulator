#include "memory.h"
#include "ppu_mode.h"
#include "joypad.h"
#include <fstream>
#include <cstring>
#include <filesystem>

//questo supporta solo ROM fino a 32kb, per ROM più grande serve un MBC o qualcosa del genere
bool Memory::loadRom(const char* filepath){
    namespace fs = std::filesystem;
    const fs::path input(filepath);

    const std::array<fs::path, 7> bases = {
        fs::current_path(),
        fs::current_path() / "..",
        fs::current_path() / ".." / "..",
        fs::current_path() / ".." / ".." / "..",
        fs::current_path() / ".." / ".." / ".." / "..",
        fs::path("."),
        fs::path("..")
    };

    std::ifstream rom;
    for (const auto& base : bases) {
        const fs::path candidate = input.is_absolute() ? input : (base / input);
        rom.open(candidate, std::ios::binary | std::ios::in);
        if (rom.is_open()) {
            break;
        }
        rom.clear();
    }

    if(!rom.is_open()){
        return false;
    }

    std::memset(rom00, 0, sizeof(rom00));
    std::memset(rom01, 0, sizeof(rom01));

    rom.read(reinterpret_cast<char*>(rom00), 0x4000);
    rom.read(reinterpret_cast<char*>(rom01), 0x4000);
    rom.close();
    return true;

}

void Memory::setPPUMode(PPUMode mode){
    this->mode = mode;
}


void Memory::LYCCompare(){
    const uint8_t ly = rawRead(0xFF44);
    const uint8_t lyc = rawRead(0xFF45);

    uint8_t stat = rawRead(0xFF41); // contenuto STAT, non indirizzo

    if (ly == lyc){
        stat |= 0x04;   // set bit 2 (LYC=LY)
    }else{
        stat &= 0xFB;   // clear bit 2
    }

    rawWrite(0xFF41, stat);
}

uint8_t Memory::readRaw(uint16_t address){
    if(address >= 0x0000 && address <= 0x3FFF)
        return rom00[address];
    if(address >= 0x4000 && address <= 0x7FFF)
        return rom01[address%0x4000];
    if(address >= 0x8000 && address <= 0x9FFF)
        return vram[address%0x2000];
    if(address >= 0xA000 && address <= 0xBFFF)
        return extRam[address%0x2000];
    if(address >= 0xC000 && address <= 0xCFFF)
        return wRam00[address%0x1000];
    if(address >= 0xD000 && address <= 0xDFFF)
        return wRam01[address%0x1000];
    if(address >= 0xE000 && address <= 0xFDFF)
        return echoRam[address%0x1E00];
    if(address >= 0xFE00 && address <= 0xFE9F)
        return oam[address%0x100];
    if(address == 0xFF00)
        return joypad->readP1(IORegisters[0x00]);
    if(address >= 0xFF00 && address <= 0xFF7F)
        return IORegisters[address%0x100];
    if(address >= 0xFF80 && address <= 0xFFFE)
        return hRam[address%0x80];
    if(address == 0xFFFF)
        return interruptEnableRegister;
    return 0xFF;
}

void Memory::writeRaw(uint16_t address, uint8_t value){
    if(address >= 0x0000 && address <= 0x3FFF){
        // rom00[address] = value;
        return;
    }
    else if(address >= 0x4000 && address <= 0x7FFF){
        // rom01[address%0x4000] = value;
        return;
    }
    else if(address >= 0x8000 && address <= 0x9FFF){
        vram[address%0x2000] = value;
    }
    else if(address >= 0xA000 && address <= 0xBFFF){
        extRam[address%0x2000] = value;
    }
    else if(address >= 0xC000 && address <= 0xCFFF){
        wRam00[address%0x1000] = value;
        echoRam[address%0x1E00] = value;
    }
    else if(address >= 0xD000 && address <= 0xDFFF){
        wRam01[address%0x1000] = value;
        if(address <= 0xDDFF)
            echoRam[address%0x1E00] = value;
    }
    else if(address >= 0xE000 && address <= 0xFDFF){
        echoRam[address % 0x1E00] = value;
    }
    else if(address >= 0xFE00 && address <= 0xFE9F){
        oam[address%0x100] = value;
    }
    else if(address >= 0xFF00 && address <= 0xFF7F){
        if(address == 0xFF00){
            IORegisters[address%0x100] = static_cast<uint8_t>(0xC0 | (value & 0x30));
        }else if(address == 0xFF04){
            IORegisters[address%0x100] = 0;
            divCounter = 0;
        }else if(address == 0xFF45 || address == 0xFF44){
            IORegisters[address%0x100] = value;
            LYCCompare();
        }else if(address == 0xFF46){
            IORegisters[address%0x100] = value;
            DMAActive = true;
            DMAIndex = 0;
            DMATicksLeft = 0;
            DMASource = (value << 8);
        }else{
            IORegisters[address%0x100] = value;
        }
    }
    else if(address >= 0xFF80 && address <= 0xFFFE){
        hRam[address%0x80] = value;
    }
    else if(address == 0xFFFF){
        interruptEnableRegister = value;
    }
}

void Memory::write(uint16_t address, uint8_t value){

    //non si scrive dentro ly cpu-side.
    if(address == 0xFF44) 
        return;

    bool oam = (address >= 0xFE00 && address <= 0xFE9F);
    bool vram = (address >= 0x8000 && address <= 0x9FFF);

    //durante DMA transfer si accede solo ad HRAM
    if (DMAActive && (address < 0xFF80 || address > 0xFFFE))
        return;
    
    if((mode == PPUMode::OamScan && oam) || (mode == PPUMode::Drawing && (oam || vram)))
        return;
    
    writeRaw(address, value);
}

void Memory::DMATransfer(int cycles){

    DMATicksLeft += cycles * 4;
    while (DMATicksLeft >= 4 && DMAIndex < 0xA0) {
        oam[DMAIndex] = readRaw(DMASource + DMAIndex);
        DMATicksLeft -= 4;
        DMAIndex++;
    }

    if(DMAIndex == 0xA0){
        DMAActive = false;
        DMASource = 0;
        DMAIndex = 0;
        return;
    }

}

void Memory::rawWrite(uint16_t address, uint8_t value){
    writeRaw(address, value);
}

void Memory::incrementDiv(){
    ++IORegisters[0xFF04%0x100];
}

void Memory::tickDiv(int tCycles){
    divCounter +=tCycles;
    while(divCounter >= 256){
        divCounter -= 256;
        incrementDiv();
    }
}

uint8_t Memory::read(uint16_t address){
    bool oam = (address >= 0xFE00 && address <= 0xFE9F);
    bool vram = (address >= 0x8000 && address <= 0x9FFF);

    if (DMAActive && (address < 0xFF80 || address > 0xFFFE))
        return 0xFF;
    
    if((mode == PPUMode::OamScan && oam) || (mode == PPUMode::Drawing && (oam || vram)))
        return 0xFF;
    
    return readRaw(address);
}

uint8_t Memory::rawRead(uint16_t address){
    return readRaw(address);
}
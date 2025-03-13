#include <iostream>
#include "Registers.h"
#include <bitset>

//ritorna 1 se la flag è settata 0 altrimenti
bool Registers::isFlagSet(uint8_t flag) {
    return this->f & flag; 
}

//aggiunge o rimuove una flag in base al valore
void Registers::setFlag(uint8_t flag, bool value) {
    if(value) {
        this->f |= flag;    // OR per settare
    } else {
        this->f &= ~flag;   // AND con NOT per resettare
    }
}

void Registers::printRegisters() {
    std::cout << "AF: 0x" << std::hex << af << std::endl;
    std::cout << "A: 0x" << std::hex << unsigned(a) << std::endl;
    std::cout << "F: 0x" << std::hex << unsigned(f) << std::endl;
    
    std::cout << "BC: 0x" << std::hex << bc << std::endl;
    std::cout << "B: 0x" << std::hex << unsigned(b) << std::endl;
    std::cout << "C: 0x" << std::hex << unsigned(c) << std::endl;
    
    std::cout << "DE: 0x" << std::hex << de << std::endl;
    std::cout << "D: 0x" << std::hex << unsigned(d) << std::endl; 
    std::cout << "E: 0x" << std::hex << unsigned(e) << std::endl;
    
    std::cout << "HL: 0x" << std::hex << hl << std::endl;
    std::cout << "H: 0x" << std::hex << unsigned(h) << std::endl;
    std::cout << "L: 0x" << std::hex << unsigned(l) << std::endl;
    
    std::cout << "SP: 0x" << std::hex << sp << std::endl;
    std::cout << "PC: 0x" << std::hex << pc << std::endl;
    
}

void Registers::printRegistersBit() {
    // 8-bit registers
    std::bitset<8> aBit(a);
    std::bitset<8> bBit(b);
    std::bitset<8> cBit(c);
    std::bitset<8> dBit(d);
    std::bitset<8> eBit(e);
    std::bitset<8> fBit(f);
    std::bitset<8> hBit(h);
    std::bitset<8> lBit(l);

    // 16-bit registers
    std::bitset<16> afBit(af);
    std::bitset<16> bcBit(bc);
    std::bitset<16> deBit(de);
    std::bitset<16> hlBit(hl);
    std::bitset<16> spBit(sp);
    std::bitset<16> pcBit(pc);

    // Print 16-bit pairs
    std::cout << "AF: 0b" << afBit << std::endl;
    std::cout << "BC: 0b" << bcBit << std::endl;
    std::cout << "DE: 0b" << deBit << std::endl;
    std::cout << "HL: 0b" << hlBit << std::endl;
    
    // Print 8-bit registers
    std::cout << "A: 0b" << aBit << std::endl;
    std::cout << "F: 0b" << fBit << std::endl;
    std::cout << "B: 0b" << bBit << std::endl;
    std::cout << "C: 0b" << cBit << std::endl;
    std::cout << "D: 0b" << dBit << std::endl;
    std::cout << "E: 0b" << eBit << std::endl;
    std::cout << "H: 0b" << hBit << std::endl;
    std::cout << "L: 0b" << lBit << std::endl;

    // Print special registers
    std::cout << "SP: 0b" << spBit << std::endl;
    std::cout << "PC: 0b" << pcBit << std::endl;
}

void Registers::printFlags(){
        std::cout << "Flags:" << std::endl;
        std::cout << "Z: " << isFlagSet(RegistersFlags::ZERO_FLAG) << std::endl;
        std::cout << "N: " << isFlagSet(RegistersFlags::SUBTRACTION_FLAG) << std::endl;  
        std::cout << "H: " << isFlagSet(RegistersFlags::HALF_CARRY_FLAG) << std::endl;
        std::cout << "C: " << isFlagSet(RegistersFlags::CARRY_FLAG) << std::endl;
}

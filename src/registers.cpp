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

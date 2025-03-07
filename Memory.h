#include <stdint.h>

class Memory
{

    public: 
    unsigned char rom00[0x4000]; //fixed ROM bank 00
    unsigned char rom01[0x4000]; //not fixed ROM bank 01-nn
    unsigned char vram[0x2000];
    unsigned char extRam[0x2000]; 
    unsigned char wRam[0x2000]; //sono due unite
    unsigned char IORegisters[0x100];
    unsigned char hRam[0x80];
    unsigned char oam[0x100]; //Object Attribute Memory. Contiene gli attributi degli oggetti mostrati a schermo

};

#include "instructions.h"
#include <iostream>

Instructions::Instructions(Registers *registers, Memory *memory, Interrupt *interrupt) {
    this->registers = registers;
    this->memory = memory;
    this->interrupt = interrupt;
}

uint16_t Instructions::readNext16Bit(){
    uint16_t immediate;

    uint8_t immediateLSB = memory->read(registers->pc);
    registers->pc++;
    uint8_t immediateMSB = memory->read(registers->pc);
    registers->pc++;
    immediate = (static_cast<uint16_t>(immediateMSB) <<8) | immediateLSB;

    return immediate;
}

uint8_t Instructions::readNext8Bit(){
    uint8_t immediate = memory->read(registers->pc);
    registers->pc++;

    return immediate;
}

void Instructions::execute(uint8_t instruction){

    switch (instruction){
    
    default:
        break;

    case 0x01: //LD BC, nn
        loadImR16(&(registers->bc));
        break;

    case 0x02: //LD BC, A
        loadAR(&(registers->bc));
        break;

    case 0x03: // INC BC
        incR16(&(registers->bc));
        break;

    case 0x04: //INC B
        incR8(&(registers->b));
        break;
    
    case 0x05: //DEC B
        decR8(&(registers->b));
        break;
        
    case 0x06: // LD B, n
        loadImR8(&(registers->b));
        break;
        
    case 0x07: //RLCA (sarebbe come uno shift left però l'ultimo bit diventa il primo)
        rlcR(&(registers->a));
        break;
        
    case 0x08: //LD SP, nn
        loadSpIm();
        break;
        
    case 0x09: //ADD HL, BC
        addHlR(&(registers->bc));
        break;
        
    case 0x0A: //LD A, BC
        loadRA(&(registers->bc));
        break;
        
    case 0x0B: //DEC BC
        decR16(&(registers->bc));
        break;
        
    case 0x0C: //INC C
        incR8(&(registers->c));
        break;
        
    case 0x0D: //DEC C
        decR8(&(registers->c));
        break;
        
    case 0x0E: //LD C, u8
        loadImR8(&(registers->c));
        break;
        
    case 0x0F: //RRCA
        rrcR(&(registers->a));
        break;
        
    case 0x10: //STOP
        stop();
        break;
        
    case 0x11: // LD DE, nn
        loadImR16(&(registers->de));
        break;

    case 0x12: // LD DE, A
        loadRA(&(registers->de));
        break;

    case 0x13: // INC DE
        incR16(&(registers->de));
        break;

    case 0x14: // INC D
        incR8(&(registers->d));
        break;

    case 0x15: // DEC D
        decR8(&(registers->d));
        break;

    case 0x16: // LD D, n
        loadImR8(&(registers->d));
        break;

    case 0x17: // RLA
        rla();
        break;

    case 0x18: // JR n (non sono per niente sicuro di questa. Da capire meglio)
        jrIm();
        break;

    case 0x19: // ADD HL, DE
        addHlR(&(registers->de));
        break;

    case 0x1A: // LD A, DE
        loadAR(&(registers->de));
        break; 

    case 0x1B: // DEC DE
        decR16(&(registers->de));
        break;

    case 0x1C: // INC E
        incR8(&(registers->e));
        break;

    case 0x1D: // DEC E
        decR8(&(registers->e));
        break;

    case 0x1E: // LD E, n
        loadImR8(&(registers->e));
        break;

    case 0x1F: // RRA
        rra();
        break;

    case 0x20: // JR NZ, n
        jrCIm(!registers->isFlagSet(RegistersFlags::ZERO_FLAG));
        break;

    case 0x21: // LD HL, nn
        loadImR16(&(registers->hl));
        break;
    
    case 0x22: // LD HL+, A
        loadAHlPlus();
        break;
    
    case 0x23: // INC HL
        incR16(&(registers->hl));
        break;
        
    case 0x24: //INC H
        incR8(&(registers->h));
        break;
        
    case 0x25: //DEC H
        decR8(&(registers->h));
        break;
        
    case 0x26: //LD H, n
        loadImR8(&(registers->h));
        break;
        
    case 0x27: //DAA
        daA();
        break;
        
    case 0x28: //JR Z, n
        jrCIm(registers->isFlagSet(RegistersFlags::ZERO_FLAG));
        break;
        
    case 0x29: //ADD HL, HL 
        addHlR(&(registers->hl));
        break;
        
    case 0x2A: //LD A, HL+
        loadHlPlusA();
        break;
        
    case 0x2B: //DEC HL
        decR16(&(registers->hl));
        break;
        
    case 0x2C: //INC L
        incR8(&(registers->l));
        break;
        
    case 0x2D: //DEC L
        decR8(&(registers->l));
        break;
        
    case 0x2E: //LD L, n
        loadImR8(&(registers->l));
        break;
        
    case 0x2F: //CPL
        cpA();
        break;
        
    case 0x30: //JR NC, n
        jrCIm(registers->isFlagSet(~RegistersFlags::CARRY_FLAG));
        break;
        
    case 0x31: // LD SP, nn
        loadImR16(&(registers->sp));
        break;

    case 0x32: //LD HL-, A
        loadAHlMinus();
        break;
        
    case 0x33: // INC SP
        incR16(&(registers->sp));
        break;

    case 0x34: //INC (HL)
        incHl();
        break;
        
    case 0x35: //DEC (HL)
        decHl();
        break;
        
    case 0x36: //LD (HL), n
        loadImHl();
        break;
        
    case 0x37: //SCF
        scf();
        break;
        
    case 0x38: //JR C, n
        jrCIm(registers->isFlagSet(RegistersFlags::CARRY_FLAG));
        break;
        
    case 0x39: //ADD HL, SP
        addHlR(&(registers->sp));
        break;
        
    case 0x3A: //LD A, (HL-)
        loadHlMinusA();
        break;
        
    case 0x3B: //DEC SP 
        decR16(&(registers->sp));
        break;
        
    case 0x3C: //INC A
        incR8(&(registers->a));
        break;
        
    case 0x3D: //DEC A
        decR8(&(registers->a));
        break;
        
    case 0x3E: //LD A, n
        loadImR8(&(registers->a));
        break;
        
    case 0x3F: //CCF
        ccf();
        break;
        
    case 0x40: //LD B, B
        loadRR8(&(registers->b), &(registers->b));
        break;
    
    case 0x41: //LD B, C
        loadRR8(&(registers->b), &(registers->c));
        break;
    
    case 0x42: //LD B, D
        loadRR8(&(registers->b), &(registers->d));
        break;
    
    case 0x43: //LD B, E
        loadRR8(&(registers->b), &(registers->e));
        break;
    
    case 0x44: //LD B, H
        loadRR8(&(registers->b), &(registers->h));
        break;
    
    case 0x45: //LD B, L
        loadRR8(&(registers->b), &(registers->l));
        break;
    
    case 0x46: //LD B, (HL)
        loadHlR(&(registers->b));
        break;
    
    case 0x47: //LD B, A
        loadRR8(&(registers->b), &(registers->a));
        break;
    
    case 0x48: //LD C, B
        loadRR8(&(registers->c), &(registers->b));
        break;
    
    case 0x49: //LD C, C
        loadRR8(&(registers->c), &(registers->c));
        break;
    
    case 0x4A: //LD C, D
        loadRR8(&(registers->c), &(registers->d));
        break;
    
    case 0x4B: //LD C, E
        loadRR8(&(registers->c), &(registers->e));
        break;
    
    case 0x4C: //LD C, H    
        loadRR8(&(registers->c), &(registers->h));
        break;
    
    case 0x4D: //LD C, L
        loadRR8(&(registers->c), &(registers->l));
        break;
    
    case 0x4E: //LD C, (hl)
        loadHlR(&(registers->c));
        break;
    
    case 0x4F: //LD C, A
        loadRR8(&(registers->b), &(registers->c));
        break;
        
    case 0x50: //LD D, B
        loadRR8(&(registers->d), &(registers->b));
        break;
    
    case 0x51: //LD D, C
        loadRR8(&(registers->d), &(registers->c));
        break;
    
    case 0x52: //LD D, D
        loadRR8(&(registers->d), &(registers->d));
        break;
    
    case 0x53: //LD D, E
        loadRR8(&(registers->d), &(registers->e));
        break;
    
    case 0x54: //LD D, H    
        loadRR8(&(registers->c), &(registers->h));
        break;
    
    case 0x55: //LD D, L
        loadRR8(&(registers->d), &(registers->l));
        break;
    
    case 0x56: //LD D, (hl)
        loadHlR(&(registers->d));
        break;
    
    case 0x57: //LD D, A
        loadRR8(&(registers->d), &(registers->c));
        break;
            
    case 0x58: //LD E, B
        loadRR8(&(registers->e), &(registers->b));
        break;
    
    case 0x59: //LD E, C
        loadRR8(&(registers->e), &(registers->c));
        break;
    
    case 0x5A: //LD E, D
        loadRR8(&(registers->e), &(registers->d));
        break;
    
    case 0x5B: //LD E, E
        loadRR8(&(registers->e), &(registers->e));
        break;
    
    case 0x5C: //LD E, H    
        loadRR8(&(registers->e), &(registers->h));
        break;
    
    case 0x5D: //LD E, L
        loadRR8(&(registers->e), &(registers->l));
        break;
    
    case 0x5E: //LD E, (hl)
        loadHlR(&(registers->e));
        break;
    
    case 0x5F: //LD E, A
        loadRR8(&(registers->e), &(registers->c));
        break;
    
    case 0x60: //LD H, B
        loadRR8(&(registers->h), &(registers->b));
        break;
    
    case 0x61: //LD H, C
        loadRR8(&(registers->h), &(registers->c));
        break;
    
    case 0x62: //LD H, D
        loadRR8(&(registers->h), &(registers->d));
        break;
    
    case 0x63: //LD H, E
        loadRR8(&(registers->h), &(registers->e));
        break;
    
    case 0x64: //LD H, H    
        loadRR8(&(registers->h), &(registers->h));
        break;
    
    case 0x65: //LD H, L
        loadRR8(&(registers->h), &(registers->l));
        break;
    
    case 0x66: //LD H, (hl)
        loadHlR(&(registers->h));
        break;
    
    case 0x67: //LD H, A
        loadRR8(&(registers->h), &(registers->c));
        break;      
    
    case 0x68: //LD L, B
        loadRR8(&(registers->l), &(registers->b));
        break;      
    
    case 0x69: //LD L, C
        loadRR8(&(registers->l), &(registers->c));
        break;      
    
    case 0x6A: //LD L, D
        loadRR8(&(registers->l), &(registers->d));
        break;      
    
    case 0x6B: //LD L, E
        loadRR8(&(registers->l), &(registers->e));
        break;      
    
    case 0x6C: //LD L, H
        loadRR8(&(registers->l), &(registers->h));
        break;      
    
    case 0x6D: //LD L, L
        loadRR8(&(registers->l), &(registers->l));
        break;      
    
    case 0x6E: //LD L, (HL)
        loadHlR(&(registers->l));
        break;      
    
    case 0x6F: //LD L, A
        loadRR8(&(registers->l), &(registers->c));
        break;      
    
    case 0x70: //LD (HL), B
        loadRHl(&(registers->b));
        break;
    
    case 0x71: //LD (HL), C
        loadRHl(&(registers->c));
        break;
    
    case 0x72: //LD (HL), D
        loadRHl(&(registers->d));
        break;
    
    case 0x73: //LD (HL), E
        loadRHl(&(registers->e));
        break;
    
    case 0x74: //LD (HL), H    
        loadRHl(&(registers->h));
        break;
    
    case 0x75: //LD (HL), L
        loadRHl(&(registers->l));
        break;
    
    case 0x76: //HALT
        halt();
        break;
    
    case 0x77: //LD (HL), A
        loadRHl(&(registers->a));
        break;      
    
    case 0x78: //LD A, B
        loadRR8(&(registers->a), &(registers->b));
        break;      
    
    case 0x79: //LD A, C
        loadRR8(&(registers->a), &(registers->c));
        break;      
    
    case 0x7A: //LD A, D
        loadRR8(&(registers->a), &(registers->d));
        break;      
    
    case 0x7B: //LD A, E
        loadRR8(&(registers->a), &(registers->e));
        break;      
    
    case 0x7C: //LD A, H
        loadRR8(&(registers->a), &(registers->h));
        break;      
    
    case 0x7D: //LD A, L
        loadRR8(&(registers->a), &(registers->l));
        break;      
    
    case 0x7E: //LD A, (HL)
        loadHlR(&(registers->a));
        break;      
    
    case 0x7F: //LD A, A
        loadRR8(&(registers->a), &(registers->a));
        break;

    case 0x80: //ADD A, B
        addRA(&(registers->b));
        break;
    
    case 0x81: //ADD A, C
        addRA(&(registers->c));
        break;
    
    case 0x82: //ADD A, D
        addRA(&(registers->d));
        break;
    
    case 0x83: //ADD A, E
        addRA(&(registers->e));
        break;
    
    case 0x84: //ADD A, H
        addRA(&(registers->h));
        break;
    
    case 0x85: //ADD A, L
        addRA(&(registers->l));
        break;
    
    case 0x86: //ADD A, (HL)
        addHlA();
        break;
    
    case 0x87: //ADD A, A
        addRA(&(registers->a));
        break;      
    
    case 0x88: //ADC A, B
        adcRA(&(registers->b));
        break;      
    
    case 0x89: //ADC A, C
        adcRA(&(registers->c));
        break;      
    
    case 0x8A: //ADC A, D
        adcRA(&(registers->d));
        break;      
    
    case 0x8B: //ADC A, E
        adcRA(&(registers->e));
        break;      
    
    case 0x8C: //ADC A, H
        adcRA(&(registers->h));
        break;      
    
    case 0x8D: //ADC A, L
        adcRA(&(registers->l));
        break;      
    
    case 0x8E: //ADC A, (HL)
        adcHlA();
        break;      
    
    case 0x8F: //ADC A, A
        adcRA(&(registers->a));
        break;
    
    case 0x90: //SUB A, B
        subRA(&(registers->b));
        break;
    
    case 0x91: //SUB A, C
        subRA(&(registers->c));
        break;
    
    case 0x92: //SUB A, D
        subRA(&(registers->d));
        break;
    
    case 0x93: //SUB A, E
        subRA(&(registers->e));
        break;
    
    case 0x94: //SUB A, H
        subRA(&(registers->h));
        break;
    
    case 0x95: //SUB A, L
        subRA(&(registers->l));
        break;
    
    case 0x96: //SUB A, (HL)
        subHlA();
        break;
    
    case 0x97: //SUB A, A
        subRA(&(registers->a));
        break;      
    
    case 0x98: //SBC A, B
        sbcRA(&(registers->b));
        break;      
    
    case 0x99: //SBC A, C
        sbcRA(&(registers->c));
        break;      
    
    case 0x9A: //SBC A, D
        sbcRA(&(registers->d));
        break;      
    
    case 0x9B: //SBC A, E
        adcRA(&(registers->e));
        break;      
    
    case 0x9C: //SBC A, H
        sbcRA(&(registers->h));
        break;      
    
    case 0x9D: //SBC A, L
        sbcRA(&(registers->l));
        break;      
    
    case 0x9E: //SBC A, (HL)
        sbcHlA();
        break;      
    
    case 0x9F: //SBC A, A
        sbcRA(&(registers->a));
        break;

    case 0xA0: //AND A, B
        andAR(&(registers->b));
        break;
    
    case 0xA1: //AND A, C
        andAR(&(registers->c));
        break;
    
    case 0xA2: //AND A, D
        andAR(&(registers->d));
        break;
    
    case 0xA3: //AND A, E
        andAR(&(registers->e));
        break;
    
    case 0xA4: //AND A, H
        andAR(&(registers->h));
        break;
    
    case 0xA5: //AND A, L
        andAR(&(registers->l));
        break;
    
    case 0xA6: //AND A, (HL)
        andAHl();
        break;
    
    case 0xA7: //AND A, A
        andAR(&(registers->a));
        break;      
    
    case 0xA8: //XOR A, B
        xorAR(&(registers->b));
        break;      
    
    case 0xA9: //XOR A, C
        xorAR(&(registers->c));
        break;      

    case 0xAA: //XOR A, D
        xorAR(&(registers->d));
        break;      
    
    case 0xAB: //XOR A, E
        xorAR(&(registers->e));
        break;      
    
    case 0xAC: //XOR A, H
        xorAR(&(registers->h));
        break;      
    
    case 0xAD: //XOR A, L
        xorAR(&(registers->l));
        break;      
    
    case 0xAE: //XOR A, (HL)
        xorAHl();
        break;      
    
    case 0xAF: //XOR A, A
        xorAR(&(registers->a));
        break;

    case 0xB0: //OR A, B
        orAR(&(registers->b));
        break;
    
    case 0xB1: //OR A, C
        orAR(&(registers->c));
        break;
    
    case 0xB2: //OR A, D
        orAR(&(registers->d));
        break;
        
    case 0xB3: //OR A, E
        orAR(&(registers->e));
        break;
        
    case 0xB4: //OR A, H
        orAR(&(registers->h));
        break;
        
    case 0xB5: //OR A, L
        orAR(&(registers->l));
        break;
        
    case 0xB6: //OR A, (HL)
        orAHl();
        break;
        
    case 0xB7: //OR A, A
        orAR(&(registers->a));
        break;      
        
    case 0xB8: //CP A, B
        cpAR(&(registers->b));
        break;      
        
    case 0xB9: //CP A, C
        cpAR(&(registers->c));
        break;      

    case 0xBA: //CP A, D
        cpAR(&(registers->d));
        break;      
        
    case 0xBB: //CP A, E
        cpAR(&(registers->e));
        break;      
        
    case 0xBC: //CP A, H
        cpAR(&(registers->h));
        break;      
        
   case 0xBD: //CP A, L
        cpAR(&(registers->l));
        break;      
        
    case 0xBE: //CP A, (HL)
        cpAHl();
        break;      
        
    case 0xBF: //CP A, A
        cpAR(&(registers->a));
        break;

    case 0xC0: //RET NZ
        retC(!registers->isFlagSet(RegistersFlags::ZERO_FLAG));
        break;
    
    case 0xC1: //POP BC
        popR(&(registers->bc));
        break;
    
    case 0xC2: //JP NZ, nn
        jpCIm(!registers->isFlagSet(RegistersFlags::ZERO_FLAG));
        break;
    
    case 0xC3: //JP nn
        jpIm();
        break;
    
    case 0xC4: //CALL NZ, nn
        callCIm(!registers->isFlagSet(RegistersFlags::ZERO_FLAG));
        break;
    
    case 0xC5: //PUSH BC
        pushR(&(registers->bc));
        break;
    
    case 0xC6: //ADD A, n
        addImA();
        break;
    
    case 0xC7: //RST 00h
        rst(0x0000);
        break;      
    
    case 0xC8: //RET Z
        retC(registers->isFlagSet(RegistersFlags::ZERO_FLAG));
        break;      
    
    case 0xC9: //RET
        ret();
        break;      
    
    case 0xCA: //JP Z, nn
        jpCIm(registers->isFlagSet(RegistersFlags::ZERO_FLAG));
        break;      
    
    case 0xCB: //PREFIX CB -> should read another 16 bits ----------------------------------------------- !!!!!!!!!!!!!!!!!!!!!!!! 
        break;      
    
    case 0xCC: //CALL Z, nn
        callCIm(registers->isFlagSet(RegistersFlags::ZERO_FLAG));
        break;      
    
    case 0xCD: //CALL nn
        callIm();
        break;      
    
    case 0xCE: //ADC A, n
        adcImA();
        break;      
    
    case 0xCF: //RST 08h
        rst(0x0008);
        break;

    case 0xD0: //RET NC
        retC(!registers->isFlagSet(RegistersFlags::CARRY_FLAG));
        break;
    
    case 0xD1: //POP DE
        popR(&(registers->de));
        break;
    
    case 0xD2: //JP NC, nn
        jpCIm(!registers->isFlagSet(RegistersFlags::CARRY_FLAG));
        break;
    
    //0xD3 is empty
    
    case 0xD4: //CALL NC, nn
        callCIm(!registers->isFlagSet(RegistersFlags::CARRY_FLAG));
        break;
    
    case 0xD5: //PUSH DE
        pushR(&(registers->de));
        break;
    
    case 0xD6: //SUB A, n
        subImA();
        break;
    
    case 0xD7: //RST 10h
        rst(0x0010);
        break;      
    
    case 0xD8: //RET C
        retC(registers->isFlagSet(RegistersFlags::CARRY_FLAG));
        break;      
    
    case 0xD9: //RETI
        retI();
        break;      
    
    case 0xDA: //JP C, nn
        jpCIm(registers->isFlagSet(RegistersFlags::CARRY_FLAG));
        break;      
    
    //0xDB is empty     
    
    case 0xDC: //SBC A, H
        callCIm(registers->isFlagSet(RegistersFlags::CARRY_FLAG));
        break;      
    
    //0xDD is empty
    
    case 0xDE: //SBC A, n
        sbcImA();
        break;      
    
    case 0xDF: //RST 18h
        rst(0x0018);
        break;

    case 0xE0: //LD (FF00+n), A
        loadAHIm();
        break;
    
    case 0xE1: //POP HL
        popR(&(registers->hl));
        break;
    
    case 0xE2: //LD (FF00+C), A
        loadAHC();
        break;
    
    //0xE3 and 0xE4 are empty
    
    case 0xE5: //PUSH HL
        pushR(&(registers->hl));
        break;
    
    case 0xE6: //AND A, n
        andAIm();
        break;
    
    case 0xE7: //RST 20h
        rst(0x0020);
        break;      
    
    case 0xE8: //ADD SP, n
        addSpE();
        break;      
    
    case 0xE9: //JP HL
        jpHl();
        break;      
    
    case 0xEA: //LD (nn), A
        loadAIm();
        break;      
    
    //0xEB, 0xEC and 0xED are empty         
    
    case 0xEE: //XOR A, n
        xorAIm();
        break;      
    
    case 0xEF: //RST 28h
        rst(0x0028);
        break;
    
    case 0xF0: //LD A, (FF00+n)
        loadHImA();
        break;
    
    case 0xF1: //POP AF
        popR(&(registers->af));
        break;
    
    case 0xF2: //LD A, (FF00+C)
        loadHCA();
        break;
    
    case 0xF3: //DI
        di();
        break;
    
    //0xF4 is empty
    
    case 0xF5: //PUSH AF
        pushR(&(registers->af));
        break;
    
    case 0xF6: //OR A, n
        orAIm();
        break;
    
    case 0xF7: //RST 30h
        rst(0x0030);
        break;      
    
    case 0xF8: //LD HL, SP+e
        loadSpEHl();
        break;      
    
    case 0xF9: //LD SP, HL
        loadHlSp();
        break;      
    
    case 0xFA: //LD A, nn
        loadImA();
        break;  
    
    case 0xFB: //EI
        ei();
        break;      
    
    //0xFC and 0xFD are empty   
    
    case 0xFE: //CP A, n
        cpAIm();
        break;      
    
    case 0xFF: //RST 38h
        rst(0x0038);
        break;
    }
}

// Basic Instructions
void Instructions::halt() {
    //da capire bene
    if(interrupt->isIMEset()){

    }
}

void Instructions::stop() {
    //non lo so da capire meglio 
    //https://gbdev.io/pandocs/Reducing_Power_Consumption.html#using-the-stop-instruction

}

void Instructions::di() {
    interrupt->setIME(false);
}

void Instructions::ei() {
    interrupt->setIME(true);
}

// Control flow

void Instructions::jpIm() {
    uint16_t address = readNext16Bit(); 
    registers->pc = address;
}

void Instructions::jpHl() {
    registers->pc = registers->hl;
}

void Instructions::jrCIm(bool condition) {
    int8_t address = (int8_t)readNext8Bit(); 
    if(condition){
        registers->pc += address;
    }
}

void Instructions::jrIm() {
    int8_t address = (int8_t)readNext8Bit(); 
    registers->pc += address;
}

void Instructions::jpCIm(bool condition) {
    uint16_t address = readNext16Bit();
    if (condition){
        registers->pc = address;
    }
}

void Instructions::callIm() {
    uint16_t jumpAddress = readNext16Bit();
    registers->sp--;
    memory->write(registers->sp, (registers->pc>>8) & 0xFF);  
    registers->sp--;
    memory->write(registers->sp, registers->pc & 0xFF);  

    registers->pc = jumpAddress;  
}


void Instructions::callCIm(bool condition) {
    if(condition){
        uint16_t address = readNext16Bit();
        uint16_t jumpAddress = readNext16Bit();
        registers->sp--;
        memory->write(registers->sp, (registers->pc>>8) & 0xFF);  
        registers->sp--;
        memory->write(registers->sp, registers->pc & 0xFF);  
    
        registers->pc = jumpAddress; 
    }
}

void Instructions::ret() {
    uint16_t pc = registers->pc;
    memory->write(registers->sp, pc & 0x00FF);
    registers->sp++;
    memory->write(registers->sp, pc & 0xFF00);
}

void Instructions::retC(bool condition) {
    uint16_t pc = registers->pc;
    if(condition){
        memory->write(registers->sp, pc & 0x00FF);
        registers->sp++;
        memory->write(registers->sp, pc & 0xFF00);
    }
}

void Instructions::retI() {
    interrupt->setIME(true);
    uint16_t pc = registers->pc;
    memory->write(registers->sp, pc & 0x00FF);
    registers->sp++;
    memory->write(registers->sp, pc & 0xFF00);
}

void Instructions::rst(uint16_t address) {
    registers->sp = address;
    registers->pc = address;
}

// Load 8 bit
void Instructions::loadRR8(uint8_t *dest, uint8_t *source) {
    
    *dest = *source;
    
}

void Instructions::loadImR8(uint8_t *dest) {
    uint8_t immediate = readNext8Bit();
    *dest = immediate;

}

void Instructions::loadHlR(uint8_t *dest) {
    uint8_t value = memory->read(registers->hl);
    *dest = value;
}

void Instructions::loadRHl(uint8_t *source) {
    memory->write(registers->hl, *source);
}

void Instructions::loadImHl() {
    uint8_t newValue = readNext8Bit();
    memory->write(registers->hl, newValue);
}

void Instructions::loadRA(uint16_t *reg) {
    registers->a = memory->read(*reg);
}

void Instructions::loadAR(uint16_t *reg) {
    *reg = registers->a;
}

void Instructions::loadImA() {
    uint16_t address = readNext16Bit();
    uint8_t value = memory->read(address);
    registers->a = value;
}

void Instructions::loadAIm() {
    uint16_t address = readNext16Bit();
    memory->write(address, registers->a);
}

void Instructions::loadHCA() {
    registers->a = memory->read(0xFF00 + registers->c);
}

void Instructions::loadAHC() {
    memory->write(0xFF00 + registers->c, registers->a);
}

void Instructions::loadAHIm() {
    uint8_t value = readNext8Bit();
    memory->write(0xFF00 + value, registers->a);
}

void Instructions::loadHImA() {
    uint8_t value = readNext8Bit();
    registers->a = 0xFF00 + value;
}

void Instructions::loadAHlMinus() {
    uint8_t a = registers->a;
    uint16_t hl = registers->hl;
    memory->write(hl, a);
    registers->hl--;
}

void Instructions::loadHlMinusA() {
    uint8_t value = memory->read(registers->hl);
    registers->a = value;
    registers->hl++;
}

void Instructions::loadAHlPlus() {
    uint8_t a = registers->a;
    uint16_t hl = registers->hl;
    memory->write(hl, a);
    registers->hl++;
}

void Instructions::loadHlPlusA() {
    uint8_t value = memory->read(registers->hl);
    registers->a = value;
    registers->hl--;
}

// Load 16 bit
void Instructions::loadImR16(uint16_t *dest) {   
    uint16_t immediate = readNext16Bit();
    *dest = immediate;
}

void Instructions::loadSpIm() {
    uint16_t immediate = readNext16Bit();
    registers->sp = immediate;
}

void Instructions::loadHlSp() {
    registers->sp = registers->hl;
}

void Instructions::pushR(uint16_t *source) {
    registers->sp--;
    memory->write(registers->sp, *source & 0x00FF);
    registers->sp--;
    memory->write(registers->sp, *source & 0xFF00);
}

void Instructions::popR(uint16_t *source) {
    memory->write(registers->sp, *source & 0x00FF);
    registers->sp++;
    memory->write(registers->sp, *source & 0xFF00);
    registers->sp++;
}

void Instructions::loadSpEHl() {
    int8_t e = (int8_t) readNext8Bit();
    uint16_t value = registers->hl + e;
    registers->sp = value;
}

// Arithmetic and logical 8 bit
void Instructions::addRA(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result = a + *reg;
    registers->a = (uint8_t) (result & 0xFF);
    
    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);

    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (*reg & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
}

void Instructions::addImA(){
    uint8_t value = readNext8Bit();
    uint8_t a = registers->a;
    uint16_t result = a + value;
    registers->a = (uint8_t) (result & 0xFF);
    
    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);

    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (value & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
    
}

void Instructions::addHlA() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result = a + hlValue;
    registers->a = (uint8_t) (result & 0xFF);
    
    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);

    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (hlValue & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
}

void Instructions::adcRA(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result;
    if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
        result = a + *reg + 1;
    }else{
        result = a + *reg;
    }
    registers->a = result & 0xFF;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);

    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (*reg & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }    
}

void Instructions::adcHlA() {

    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result;
    if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
        result = a + hlValue + 1;
    }else{
        result = a + hlValue;
    }
    registers->a = result & 0xFF;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);

    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (hlValue & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }    
}

void Instructions::adcImA() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint16_t result;
    if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
        result = a + value + 1;
    }else{
        result = a + value;
    }
    registers->a = result & 0xFF;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);

    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (value & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }    
}

void Instructions::subRA(uint8_t *reg) {

    uint8_t a = registers->a;
    uint16_t result = a - *reg;
    registers->a = result & 0xFF;

    //overflow
    if(*reg > a){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //resto per i primi 4 bit. controllo se i primi 4 bit del registro a cui sommo sono maggiori di A
    if((a & 0xF) < (*reg & 0xF)){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
}

void Instructions::subHlA() {
    
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result = a - hlValue;
    registers->a = result & 0xFF;

    //overflow
    if(hlValue > a){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //resto per i primi 4 bit. controllo se i primi 4 bit del registro a cui sommo sono maggiori di A
    if((a & 0xF) < (hlValue & 0xF)){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
}

void Instructions::subImA() {
    
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint16_t result = a - value;
    registers->a = result & 0xFF;

    //overflow
    if(value > a){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //resto per i primi 4 bit. controllo se i primi 4 bit del registro a cui sommo sono maggiori di A
    if((a & 0xF) < (value & 0xF)){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
}

void Instructions::sbcRA(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result;
    if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
        result = a - *reg - 1;
    }else {
        result = a - *reg;
    }

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }
    registers->setFlag(RegistersFlags::ZERO_FLAG, result & 0xFF == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) < (*reg & 0xF)){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
    
    registers->a = result && 0xFF;
}

void Instructions::sbcHlA() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result;
    if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
        result = a - hlValue - 1;
    }else {
        result = a - hlValue;
    }

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }
    registers->setFlag(RegistersFlags::ZERO_FLAG, result & 0xFF == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) < (hlValue & 0xF)){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
    
    registers->a = result && 0xFF;
}

void Instructions::sbcImA() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint16_t result;
    if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
        result = a - value - 1;
    }else {
        result = a - value;
    }

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }
    registers->setFlag(RegistersFlags::ZERO_FLAG, result & 0xFF == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) < (value & 0xF)){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
    
    registers->a = result && 0xFF;
}

void Instructions::cpAR(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result = a - *reg;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }
    registers->setFlag(RegistersFlags::ZERO_FLAG, result & 0xFF == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) < (*reg & 0xF)){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
}

void Instructions::cpAHl() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result = a - hlValue;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }
    registers->setFlag(RegistersFlags::ZERO_FLAG, result & 0xFF == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) < (hlValue & 0xF)){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
}

void Instructions::cpAIm() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint16_t result = a - value;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }
    registers->setFlag(RegistersFlags::ZERO_FLAG, result & 0xFF == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) < (value & 0xF)){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
}

void Instructions::incR8(uint8_t *reg) {
    uint8_t oldRegValue = *reg;
    (*reg)++;

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((oldRegValue & 0x0F) + 1 > 0x0F){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }else{
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    }
}

void Instructions::incHl() {
    uint8_t value = memory->read(registers->hl);
    value++;
    memory->write(registers->hl, value);
}

void Instructions::decR8(uint8_t *reg) {
    uint8_t oldRegValue = *reg;
    (*reg)--;

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //half carry qui indica il prestito dal quarto bit. Il prestito avviene solo se i primi 4 bit sono a 0
    if((oldRegValue & 0x0F) == 0){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }else{
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    }
}

void Instructions::decHl() {
    uint8_t value = memory->read(registers->hl);
    value--;
    memory->write(registers->hl, value);
}

void Instructions::andAR(uint8_t *reg) {
    uint8_t a = registers->a & *reg;
    registers->a = a;

    registers->setFlag(RegistersFlags::ZERO_FLAG, a & 0xFF == 0); //true se 0 altrimenti false
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::andAHl() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint8_t res = a & hlValue;
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, (res & 0xFF) == 0); //true se 0 altrimenti false
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::andAIm() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint8_t res = a & value;
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, (res & 0xFF) == 0); //true se 0 altrimenti false
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::orAR(uint8_t *reg) {
    uint8_t a = registers->a;
    uint8_t res = registers->a | *reg;
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

}

void Instructions::orAHl() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint8_t res = a | hlValue;
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::orAIm() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint8_t res = a | value;
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::xorAR(uint8_t *reg) {
    uint8_t a = registers->a;
    uint8_t res = a xor *reg;
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::xorAHl() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint8_t res = (a & ~hlValue) | (~a & hlValue);
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::xorAIm() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint8_t res = (a & ~value) | (~a & value);
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::ccf() {
    registers->setFlag(RegistersFlags::CARRY_FLAG, !registers->isFlagSet(RegistersFlags::CARRY_FLAG));
}

void Instructions::scf() {
    registers->setFlag(RegistersFlags::CARRY_FLAG, true);
}


void Instructions::daA() { 
    uint8_t adj = 0; 
    if(registers->isFlagSet(RegistersFlags::SUBTRACTION_FLAG)){
        if(registers->isFlagSet(RegistersFlags::HALF_CARRY_FLAG)){
            adj += 0x6;
        }else if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
            adj+= 0x60;
        }
        registers->a -= adj;
    }else{
        if(registers->isFlagSet(RegistersFlags::HALF_CARRY_FLAG) || (registers->a & 0xF) > 0x9){
            adj += 0x6;
        }else if (registers->isFlagSet(RegistersFlags::CARRY_FLAG) || registers->a > 0x99){
            adj += 0x60;
        }
        registers->a += adj;
    }

    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::ZERO_FLAG, registers->a == 0);
    registers->setFlag(RegistersFlags::CARRY_FLAG, true); //? ? ? DA CAPIRE
}

void Instructions::cpA() {
    registers->a = ~registers->a;
}

// Arithmetic and logical 16 bit
void Instructions::incR16(uint16_t *reg) {
    uint16_t oldRegValue = *reg;
    (*reg)++;
}

void Instructions::decR16(uint16_t *reg) {
    uint16_t oldRegValue = *reg;
    (*reg)--;
}

void Instructions::addHlR(uint16_t *reg) {
    uint16_t hl = registers->hl;
    uint16_t result = hl + *reg;
    registers->a = result && 0xFF;

    //overflow from bit 15
    if((hl & 0x8000) & (*reg & 0x8000) == 1){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);

    //overflow from bit 11
    if((hl & 0x0400) + (*reg & 0x0400) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
}

void Instructions::addSpE() {
    uint16_t sp = registers->sp;
    int8_t e = (int8_t) readNext8Bit();
    uint16_t result = sp + e;
    registers->a = result && 0xFF;

    //overflow from bit 3
    if((sp & 0x4) & (e & 0x4) == 1){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }

    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);

    //overflow from bit 7
    if((e & 0x40) + (e & 0x40) == 1){
        registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    }
}

// Bit operations

void Instructions::rla() {
    uint8_t a = registers->a;
    uint8_t bit7 = (a & 0x80) >> 7;
    if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
        a = a << 1 | 1;
    }else{
        a = a << 1;
    }
    registers->a = a;

    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7==1);
    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
}

void Instructions::rra() {
    uint8_t oldValue = registers->a;
    uint8_t bit1 = (oldValue & 0x01) << 7;
    registers->a = oldValue >> 1 | bit1;

    registers->setFlag(RegistersFlags::CARRY_FLAG, bit1);
    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
}

void Instructions::rlcR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    *reg = oldValue << 1 | bit7;

    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7==1);
}

void Instructions::rlcHl() {}

void Instructions::rrcR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t bit1 = (oldValue & 0x01) << 7;
    *reg = oldValue >> 1 | bit1;

    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit1==1);
}

void Instructions::rrcHl() {}
void Instructions::rrR(uint8_t *reg) {}
void Instructions::rrHl() {}
void Instructions::slaR(uint8_t *reg) {}
void Instructions::slaHl() {}
void Instructions::sraR(uint8_t *reg) {}
void Instructions::sraHl() {}
void Instructions::swapR(uint8_t *reg) {}
void Instructions::swapHl() {}
void Instructions::srlR(uint8_t *reg) {}
void Instructions::srlHl() {}
void Instructions::bitBR(uint8_t *reg) {}
void Instructions::bitBHl() {}
void Instructions::resBR(uint8_t *reg) {}
void Instructions::resBHl() {}
void Instructions::setBR(uint8_t *reg) {}
void Instructions::setBHl() {}
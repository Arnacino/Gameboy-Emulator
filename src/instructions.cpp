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

// DA IMPLEMENTARE I CICLI CORRETTI PER OGNI ISTRUZIONE...
int Instructions::execute(uint8_t instruction){

    switch (instruction){
    
    default:
        return 1;
        break;

    case 0x01: //LD BC, nn
        loadImR16(&(registers->bc));
        return 3;
        break;

    case 0x02: //LD BC, A
        loadRA(&(registers->bc));
        return 2;
        break;

    case 0x03: // INC BC
        incR16(&(registers->bc));
        return 2;
        break;

    case 0x04: //INC B
        incR8(&(registers->b));
        return 1;
        break;
    
    case 0x05: //DEC B
        decR8(&(registers->b));
        return 1;
        break;
        
    case 0x06: // LD B, n
        loadImR8(&(registers->b));
        return 2;
        break;
        
    case 0x07: //RLCA (sarebbe come uno shift left però l'ultimo bit diventa il primo)
        rlca();
        return 1;
        break;
        
    case 0x08: //LD SP, nn
        loadSpIm();
        return 5;
        break;
        
    case 0x09: //ADD HL, BC
        addHlR(&(registers->bc));
        return 2;
        break;
        
    case 0x0A: //LD A, BC
        loadAR(&(registers->bc));
        return 2;
        break;
        
    case 0x0B: //DEC BC
        decR16(&(registers->bc));
        return 2;
        break;
        
    case 0x0C: //INC C
        incR8(&(registers->c));
        return 1;
        break;
        
    case 0x0D: //DEC C
        decR8(&(registers->c));
        return 1;
        break;
        
    case 0x0E: //LD C, u8
        loadImR8(&(registers->c));
        return 2;
        break;
        
    case 0x0F: //RRCA
        rrca();
        return 1;
        break;
        
    case 0x10: //STOP ---------------------------- DA IMPLEMENTARE (?)
        stop();
        break;
        
    case 0x11: // LD DE, nn
        loadImR16(&(registers->de));
        return 3;
        break;

    case 0x12: // LD DE, A
        loadRA(&(registers->de));
        return 2;
        break;

    case 0x13: // INC DE
        incR16(&(registers->de));
        return 2;
        break;

    case 0x14: // INC D
        incR8(&(registers->d));
        return 1;
        break;

    case 0x15: // DEC D
        decR8(&(registers->d));
        return 1;
        break;

    case 0x16: // LD D, n
        loadImR8(&(registers->d));
        return 2;
        break;

    case 0x17: // RLA
        rla();
        return 1;
        break;

    case 0x18: // JR n (non sono per niente sicuro di questa. Da capire meglio)
        jrIm();
        return 3;
        break;

    case 0x19: // ADD HL, DE
        addHlR(&(registers->de));
        return 2;
        break;

    case 0x1A: // LD A, DE
        loadAR(&(registers->de));
        return 2;
        break; 

    case 0x1B: // DEC DE
        decR16(&(registers->de));
        return 2;
        break;

    case 0x1C: // INC E
        incR8(&(registers->e));
        return 1;
        break;

    case 0x1D: // DEC E
        decR8(&(registers->e));
        return 1;
        break;

    case 0x1E: // LD E, n
        loadImR8(&(registers->e));
        return 2;
        break;

    case 0x1F: // RRA
        rra();
        return 1;
        break;

    case 0x20: { // JR NZ, n
        bool condition = !registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        jrCIm(condition);
        if(condition){
            return 3;
        }
        return 2;
        break;
    }

    case 0x21: // LD HL, nn
        loadImR16(&(registers->hl));
        return 3;
        break;
    
    case 0x22: // LD HL+, A
        loadAHlPlus();
        return 2;
        break;
    
    case 0x23: // INC HL
        incR16(&(registers->hl));
        return 2;
        break;
        
    case 0x24: //INC H
        incR8(&(registers->h));
        return 1;
        break;
        
    case 0x25: //DEC H
        decR8(&(registers->h));
        return 1;
        break;
        
    case 0x26: //LD H, n
        loadImR8(&(registers->h));
        return 2;
        break;
        
    case 0x27: //DAA
        daA();
        return 1;
        break;
        
    case 0x28: {  //JR Z, n
        bool condition = registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        jrCIm(condition);
        if(condition){
            return 3;
        }
        return 2;
        break;
    }

    case 0x29: //ADD HL, HL 
        addHlR(&(registers->hl));
        return 2;
        break;
        
    case 0x2A: //LD A, HL+
        loadHlPlusA();
        return 2;
        break;
        
    case 0x2B: //DEC HL
        decR16(&(registers->hl));
        return 2;
        break;
        
    case 0x2C: //INC L
        incR8(&(registers->l));
        return 1;
        break;
        
    case 0x2D: //DEC L
        decR8(&(registers->l));
        return 1;
        break;
        
    case 0x2E: //LD L, n
        loadImR8(&(registers->l));
        return 2;
        break;
        
    case 0x2F: //CPL
        cpA();
        return 1;
        break;
        
    case 0x30:{ //JR NC, n
        bool condition = !registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        jrCIm(condition);
        if (condition){
            return 3;
        }
        return 2;
        break;
    }
        
    case 0x31: // LD SP, nn
        loadImR16(&(registers->sp));
        return 3;
        break;

    case 0x32: //LD HL-, A
        loadAHlMinus();
        return 2;
        break;
        
    case 0x33: // INC SP
        incR16(&(registers->sp));
        return 2;
        break;

    case 0x34: //INC (HL)
        incHl();
        return 3;
        break;
        
    case 0x35: //DEC (HL)
        decHl();
        return 2;
        break;
        
    case 0x36: //LD (HL), n
        loadImHl();
        return 3;
        break;
        
    case 0x37: //SCF
        scf();
        return 1;
        break;
        
    case 0x38:{ //JR C, n
        bool condition = registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        jrCIm(condition);
        if(condition){
            return 3;
        }
        return 2;
        break;
    }

    case 0x39: //ADD HL, SP
        addHlR(&(registers->sp));
        return 2;
        break;
        
    case 0x3A: //LD A, (HL-)
        loadHlMinusA();
        return 2;
        break;
        
    case 0x3B: //DEC SP 
        decR16(&(registers->sp));
        return 2;
        break;
        
    case 0x3C: //INC A
        incR8(&(registers->a));
        return 1;
        break;
        
    case 0x3D: //DEC A
        decR8(&(registers->a));
        return 1;
        break;
        
    case 0x3E: //LD A, n
        loadImR8(&(registers->a));
        return 2;
        break;
        
    case 0x3F: //CCF
        ccf();
        return 1;
        break;
        
    case 0x40: //LD B, B
        loadRR8(&(registers->b), &(registers->b));
        return 1;
        break;
    
    case 0x41: //LD B, C
        loadRR8(&(registers->b), &(registers->c));
        return 1;
        break;
    
    case 0x42: //LD B, D
        loadRR8(&(registers->b), &(registers->d));
        return 1;
        break;
    
    case 0x43: //LD B, E
        loadRR8(&(registers->b), &(registers->e));
        return 1;
        break;
    
    case 0x44: //LD B, H
        loadRR8(&(registers->b), &(registers->h));
        return 1;
        break;
    
    case 0x45: //LD B, L
        loadRR8(&(registers->b), &(registers->l));
        return 1;
        break;
    
    case 0x46: //LD B, (HL)
        loadHlR(&(registers->b));
        return 2;
        break;
    
    case 0x47: //LD B, A
        loadRR8(&(registers->b), &(registers->a));
        return 1;
        break;
    
    case 0x48: //LD C, B
        loadRR8(&(registers->c), &(registers->b));
        return 1;
        break;
    
    case 0x49: //LD C, C
        loadRR8(&(registers->c), &(registers->c));
        return 1;
        break;
    
    case 0x4A: //LD C, D
        loadRR8(&(registers->c), &(registers->d));
        return 1;
        break;
    
    case 0x4B: //LD C, E
        loadRR8(&(registers->c), &(registers->e));
        return 1;
        break;
    
    case 0x4C: //LD C, H    
        loadRR8(&(registers->c), &(registers->h));
        return 1;
        break;
    
    case 0x4D: //LD C, L
        loadRR8(&(registers->c), &(registers->l));
        return 1;
        break;
    
    case 0x4E: //LD C, (hl)
        loadHlR(&(registers->c));
        return 2;
        break;
    
    case 0x4F: //LD C, A
        loadRR8(&(registers->c), &(registers->a));
        return 1;
        break;
        
    case 0x50: //LD D, B
        loadRR8(&(registers->d), &(registers->b));
        return 1;
        break;
    
    case 0x51: //LD D, C
        loadRR8(&(registers->d), &(registers->c));
        return 1;
        break;
    
    case 0x52: //LD D, D
        loadRR8(&(registers->d), &(registers->d));
        return 1;
        break;
    
    case 0x53: //LD D, E
        loadRR8(&(registers->d), &(registers->e));
        return 1;
        break;
    
    case 0x54: //LD D, H    
        loadRR8(&(registers->d), &(registers->h));
        return 1;
        break;
    
    case 0x55: //LD D, L
        loadRR8(&(registers->d), &(registers->l));
        return 1;
        break;
    
    case 0x56: //LD D, (hl)
        loadHlR(&(registers->d));
        return 2;
        break;
    
    case 0x57: //LD D, A
        loadRR8(&(registers->d), &(registers->a));
        return 1;
        break;
            
    case 0x58: //LD E, B
        loadRR8(&(registers->e), &(registers->b));
        return 1;
        break;
    
    case 0x59: //LD E, C
        loadRR8(&(registers->e), &(registers->c));
        return 1;
        break;
    
    case 0x5A: //LD E, D
        loadRR8(&(registers->e), &(registers->d));
        return 1;
        break;
    
    case 0x5B: //LD E, E
        loadRR8(&(registers->e), &(registers->e));
        return 1;
        break;
    
    case 0x5C: //LD E, H    
        loadRR8(&(registers->e), &(registers->h));
        return 1;
        break;
    
    case 0x5D: //LD E, L
        loadRR8(&(registers->e), &(registers->l));
        return 1;
        break;
    
    case 0x5E: //LD E, (hl)
        loadHlR(&(registers->e));
        return 2;
        break;
    
    case 0x5F: //LD E, A
        loadRR8(&(registers->e), &(registers->a));
        return 1;
        break;
    
    case 0x60: //LD H, B
        loadRR8(&(registers->h), &(registers->b));
        return 1;
        break;
    
    case 0x61: //LD H, C
        loadRR8(&(registers->h), &(registers->c));
        return 1;
        break;
    
    case 0x62: //LD H, D
        loadRR8(&(registers->h), &(registers->d));
        return 1;
        break;
    
    case 0x63: //LD H, E
        loadRR8(&(registers->h), &(registers->e));
        return 1;
        break;
    
    case 0x64: //LD H, H    
        loadRR8(&(registers->h), &(registers->h));
        return 1;
        break;
    
    case 0x65: //LD H, L
        loadRR8(&(registers->h), &(registers->l));
        return 1;
        break;
    
    case 0x66: //LD H, (hl)
        loadHlR(&(registers->h));
        return 2;
        break;
    
    case 0x67: //LD H, A
        loadRR8(&(registers->h), &(registers->a));
        return 1;
        break;      
    
    case 0x68: //LD L, B
        loadRR8(&(registers->l), &(registers->b));
        return 1;
        break;      
    
    case 0x69: //LD L, C
        loadRR8(&(registers->l), &(registers->c));
        return 1;
        break;      
    
    case 0x6A: //LD L, D
        loadRR8(&(registers->l), &(registers->d));
        return 1;
        break;      
    
    case 0x6B: //LD L, E
        loadRR8(&(registers->l), &(registers->e));
        return 1;
        break;      
    
    case 0x6C: //LD L, H
        loadRR8(&(registers->l), &(registers->h));
        return 1;
        break;      
    
    case 0x6D: //LD L, L
        loadRR8(&(registers->l), &(registers->l));
        return 1;
        break;      
    
    case 0x6E: //LD L, (HL)
        loadHlR(&(registers->l));
        return 2;
        break;      
    
    case 0x6F: //LD L, A
        loadRR8(&(registers->l), &(registers->a));
        return 1;
        break;      
    
    case 0x70: //LD (HL), B
        loadRHl(&(registers->b));
        return 2;
        break;
    
    case 0x71: //LD (HL), C
        loadRHl(&(registers->c));
        return 2;
        break;
    
    case 0x72: //LD (HL), D
        loadRHl(&(registers->d));
        return 2;
        break;
    
    case 0x73: //LD (HL), E
        loadRHl(&(registers->e));
        return 2;
        break;
    
    case 0x74: //LD (HL), H    
        loadRHl(&(registers->h));
        return 2;
        break;
    
    case 0x75: //LD (HL), L
        loadRHl(&(registers->l));
        return 2;
        break;
    
    case 0x76: //HALT
        halt();
        //????????????
        break;
    
    case 0x77: //LD (HL), A
        loadRHl(&(registers->a));
        return 2;
        break;      
    
    case 0x78: //LD A, B
        loadRR8(&(registers->a), &(registers->b));
        return 1;
        break;      
    
    case 0x79: //LD A, C
        loadRR8(&(registers->a), &(registers->c));
        return 1;
        break;      
    
    case 0x7A: //LD A, D
        loadRR8(&(registers->a), &(registers->d));
        return 1;
        break;      
    
    case 0x7B: //LD A, E
        loadRR8(&(registers->a), &(registers->e));
        return 1;
        break;      
    
    case 0x7C: //LD A, H
        loadRR8(&(registers->a), &(registers->h));
        return 1;
        break;      
    
    case 0x7D: //LD A, L
        loadRR8(&(registers->a), &(registers->l));
        return 1;
        break;      
    
    case 0x7E: //LD A, (HL)
        loadHlR(&(registers->a));
        return 2;
        break;      
    
    case 0x7F: //LD A, A
        loadRR8(&(registers->a), &(registers->a));
        return 1;
        break;

    case 0x80: //ADD A, B
        addRA(&(registers->b));
        return 1;
        break;
    
    case 0x81: //ADD A, C
        addRA(&(registers->c));
        return 1;
        break;
    
    case 0x82: //ADD A, D
        addRA(&(registers->d));
        return 1;
        break;
    
    case 0x83: //ADD A, E
        addRA(&(registers->e));
        return 1;
        break;
    
    case 0x84: //ADD A, H
        addRA(&(registers->h));
        return 1;
        break;
    
    case 0x85: //ADD A, L
        addRA(&(registers->l));
        return 1;
        break;
    
    case 0x86: //ADD A, (HL)
        addHlA();
        return 2;
        break;
    
    case 0x87: //ADD A, A
        addRA(&(registers->a));
        return 1;
        break;      
    
    case 0x88: //ADC A, B
        adcRA(&(registers->b));
        return 1;
        break;      
    
    case 0x89: //ADC A, C
        adcRA(&(registers->c));
        return 1;
        break;      
    
    case 0x8A: //ADC A, D
        adcRA(&(registers->d));
        return 1;
        break;      
    
    case 0x8B: //ADC A, E
        adcRA(&(registers->e));
        return 1;
        break;      
    
    case 0x8C: //ADC A, H
        adcRA(&(registers->h));
        return 1;
        break;      
    
    case 0x8D: //ADC A, L
        adcRA(&(registers->l));
        return 1;
        break;      
    
    case 0x8E: //ADC A, (HL)
        adcHlA();
        return 2;
        break;      
    
    case 0x8F: //ADC A, A
        adcRA(&(registers->a));
        return 1;
        break;
    
    case 0x90: //SUB A, B
        subRA(&(registers->b));
        return 1;
        break;
    
    case 0x91: //SUB A, C
        subRA(&(registers->c));
        return 1;
        break;
    
    case 0x92: //SUB A, D
        subRA(&(registers->d));
        return 1;
        break;
    
    case 0x93: //SUB A, E
        subRA(&(registers->e));
        return 1;
        break;
    
    case 0x94: //SUB A, H
        subRA(&(registers->h));
        return 1;
        break;
    
    case 0x95: //SUB A, L
        subRA(&(registers->l));
        return 1;
        break;
    
    case 0x96: //SUB A, (HL)
        subHlA();
        return 2;
        break;
    
    case 0x97: //SUB A, A
        subRA(&(registers->a));
        return 1;
        break;      
    
    case 0x98: //SBC A, B
        sbcRA(&(registers->b));
        return 1;
        break;      
    
    case 0x99: //SBC A, C
        sbcRA(&(registers->c));
        return 1;
        break;      
    
    case 0x9A: //SBC A, D
        sbcRA(&(registers->d));
        return 1;
        break;      
    
    case 0x9B: //SBC A, E
        sbcRA(&(registers->e));
        return 1;
        break;
    
    case 0x9C: //SBC A, H
        sbcRA(&(registers->h));
        return 1;
        break;      
    
    case 0x9D: //SBC A, L
        sbcRA(&(registers->l));
        return 1;
        break;      
    
    case 0x9E: //SBC A, (HL)
        sbcHlA();
        return 2;
        break;      
    
    case 0x9F: //SBC A, A
        sbcRA(&(registers->a));
        return 1;
        break;

    case 0xA0: //AND A, B
        andAR(&(registers->b));
        return 1;
        break;
    
    case 0xA1: //AND A, C
        andAR(&(registers->c));
        return 1;
        break;
    
    case 0xA2: //AND A, D
        andAR(&(registers->d));
        return 1;
        break;
    
    case 0xA3: //AND A, E
        andAR(&(registers->e));
        return 1;
        break;
    
    case 0xA4: //AND A, H
        andAR(&(registers->h));
        return 1;
        break;
    
    case 0xA5: //AND A, L
        andAR(&(registers->l));
        return 1;
        break;
    
    case 0xA6: //AND A, (HL)
        andAHl();
        return 2;
        break;
    
    case 0xA7: //AND A, A
        andAR(&(registers->a));
        return 1;
        break;      
    
    case 0xA8: //XOR A, B
        xorAR(&(registers->b));
        return 1;
        break;      
    
    case 0xA9: //XOR A, C
        xorAR(&(registers->c));
        return 1;
        break;      

    case 0xAA: //XOR A, D
        xorAR(&(registers->d));
        return 1;
        break;      
    
    case 0xAB: //XOR A, E
        xorAR(&(registers->e));
        return 1;
        break;      
    
    case 0xAC: //XOR A, H
        xorAR(&(registers->h));
        return 1;
        break;      
    
    case 0xAD: //XOR A, L
        xorAR(&(registers->l));
        return 1;
        break;      
    
    case 0xAE: //XOR A, (HL)
        xorAHl();
        return 2;
        break;      
    
    case 0xAF: //XOR A, A
        xorAR(&(registers->a));
        return 1;
        break;

    case 0xB0: //OR A, B
        orAR(&(registers->b));
        return 1;
        break;
    
    case 0xB1: //OR A, C
        orAR(&(registers->c));
        return 1;
        break;
    
    case 0xB2: //OR A, D
        orAR(&(registers->d));
        return 1;
        break;
        
    case 0xB3: //OR A, E
        orAR(&(registers->e));
        return 1;
        break;
        
    case 0xB4: //OR A, H
        orAR(&(registers->h));
        return 1;
        break;
        
    case 0xB5: //OR A, L
        orAR(&(registers->l));
        return 1;
        break;
        
    case 0xB6: //OR A, (HL)
        orAHl();
        return 2;
        break;
        
    case 0xB7: //OR A, A
        orAR(&(registers->a));
        return 1;
        break;      
        
    case 0xB8: //CP A, B
        cpAR(&(registers->b));
        return 1;
        break;      
        
    case 0xB9: //CP A, C
        cpAR(&(registers->c));
        return 1;
        break;      

    case 0xBA: //CP A, D
        cpAR(&(registers->d));
        return 1;
        break;      
        
    case 0xBB: //CP A, E
        cpAR(&(registers->e));
        return 1;
        break;      
        
    case 0xBC: //CP A, H
        cpAR(&(registers->h));
        return 1;
        break;      
        
   case 0xBD: //CP A, L
        cpAR(&(registers->l));
        return 1;
        break;      
        
    case 0xBE: //CP A, (HL)
        cpAHl();
        return 2;
        break;      
        
    case 0xBF: //CP A, A
        cpAR(&(registers->a));
        return 1;
        break;

    case 0xC0: { //RET NZ
        bool condition = !registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        retC(condition);
        if(condition){
            return 5;
        }
        return 2;
        break;
    }
    
    case 0xC1: //POP BC
        popR(&(registers->bc));
        return 3;
        break;
    
    case 0xC2: { //JP NZ, nn
        bool condition = !registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        jpCIm(condition);
        if(condition){
            return 4;
        }
        return 3;
        break;
    }

    case 0xC3: //JP nn
        jpIm();
        return 4;
        break;
    
    case 0xC4: { //CALL NZ, nn
        bool condition = !registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        callCIm(condition);
        if(condition){
            return 6;
        }
        return 3;
        break;
    }

    case 0xC5: //PUSH BC
        pushR(&(registers->bc));
        return 4;
        break;
    
    case 0xC6: //ADD A, n
        addImA();
        return 2;
        break;
    
    case 0xC7: //RST 00h
        rst(0x0000);
        return 4;
        break;      
    
    case 0xC8: { //RET Z
        bool condition = registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        retC(condition);
        if(condition){
            return 5;
        }
        return 2;
        break;  
    }    
    
    case 0xC9: //RET
        ret();
        return 4;
        break;      
    
    case 0xCA: { //JP Z, nn
        bool condition = registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        jpCIm(condition);
        if(condition){
            return 4;
        }
        return 3;
        break;  
    }    
    
    case 0xCB: { //PREFIX CB 
        uint8_t nextInstruction = readNext8Bit();
        switch(nextInstruction) {
            case 0x00:
                rlcR(&(registers->b));
                return 2;
                break;
            
            case 0x01:
                rlcR(&(registers->c));
                return 2;
                break;
            
            case 0x02:
                rlcR(&(registers->d));
                return 2;
                break;
            
            case 0x03:
                rlcR(&(registers->e));
                return 2;
                break;
            
            case 0x04:
                rlcR(&(registers->h));
                return 2;
                break;
            
            case 0x05:
                rlcR(&(registers->l));
                return 2;
                break;

            case 0x06:
                rlcHl();
                return 4;
                break;

            case 0x07:
                rlcR(&(registers->a));
                return 2;
                break;

            case 0x08:
                rrcR(&(registers->b));
                return 2;
                break;

            case 0x09:
                rrcR(&(registers->c));
                return 2;
                break;

            case 0x0A:
                rrcR(&(registers->d));
                return 2;
                break;

            case 0x0B:
                rrcR(&(registers->e));
                return 2;
                break;

            case 0x0C:
                rrcR(&(registers->h));
                return 2;
                break;

            case 0x0D:
                rrcR(&(registers->l));
                return 2;
                break;

            case 0x0E:
                rrcHl();
                return 4;
                break;

            case 0x0F:
                rrcR(&(registers->a));
                return 2;
                break;

            case 0x10:
                rlR(&(registers->b));
                return 2;
                break;

            case 0x11:
                rlR(&(registers->c));
                return 2;
                break;

            case 0x12:
                rlR(&(registers->d));
                return 2;
                break;

            case 0x13:
                rlR(&(registers->e));
                return 2;
                break;

            case 0x14:
                rlR(&(registers->h));
                return 2;
                break;

            case 0x15:
                rlR(&(registers->l));
                return 2;
                break;

            case 0x16:
                rlHl();
                return 4;
                break;

            case 0x17:
                rlR(&(registers->a));
                return 2;
                break;

            case 0x18:
                rrR(&(registers->b));
                return 2;
                break;

            case 0x19:
                rrR(&(registers->c));
                return 2;
                break;

            case 0x1A:
                rrR(&(registers->d));
                return 2;
                break;

            case 0x1B:
                rrR(&(registers->e));
                return 2;
                break;

            case 0x1C:
                rrR(&(registers->h));
                return 2;
                break;

            case 0x1D:
                rrR(&(registers->l));
                return 2;
                break;

            case 0x1E:
                rrHl();
                return 4;
                break;

            case 0x1F:
                rrR(&(registers->a));
                return 2;
                break;

            case 0x20:
                slaR(&(registers->b));
                return 2;
                break;

            case 0x21:
                slaR(&(registers->c));
                return 2;
                break;

            case 0x22:
                slaR(&(registers->d));
                return 2;
                break;

            case 0x23:
                slaR(&(registers->e));
                return 2;
                break;

            case 0x24:
                slaR(&(registers->h));
                return 2;
                break;

            case 0x25:
                slaR(&(registers->l));
                return 2;
                break;

            case 0x26:
                slaHl();
                return 4;
                break;

            case 0x27:
                slaR(&(registers->a));
                return 2;
                break;

            case 0x28:
                sraR(&(registers->b));
                return 2;
                break;

            case 0x29:
                sraR(&(registers->c));
                return 2;
                break;

            case 0x2A:
                sraR(&(registers->d));
                return 2;
                break;

            case 0x2B:
                sraR(&(registers->e));
                return 2;
                break;

            case 0x2C:
                sraR(&(registers->h));
                return 2;
                break;

            case 0x2D:
                sraR(&(registers->l));
                return 2;
                break;

            case 0x2E:
                sraHl();
                return 4;
                break;

            case 0x2F:
                sraR(&(registers->a));
                return 2;
                break;

            case 0x30:
                swapR(&(registers->b));
                return 2;
                break;

            case 0x31:
                swapR(&(registers->c));
                return 2;
                break;

            case 0x32:
                swapR(&(registers->d));
                return 2;
                break;

            case 0x33:
                swapR(&(registers->e));
                return 2;
                break;

            case 0x34:
                swapR(&(registers->h));
                return 2;
                break;

            case 0x35:
                swapR(&(registers->l));
                return 2;
                break;

            case 0x36:
                swapHl();
                return 4;
                break;

            case 0x37:
                swapR(&(registers->a));
                return 2;
                break;

            case 0x38:
                srlR(&(registers->b));
                return 2;
                break;

            case 0x39:
                srlR(&(registers->c));
                return 2;
                break;

            case 0x3A:
                srlR(&(registers->d));
                return 2;
                break;

            case 0x3B:
                srlR(&(registers->e));
                return 2;
                break;

            case 0x3C:
                srlR(&(registers->h));
                return 2;
                break;

            case 0x3D:
                srlR(&(registers->l));
                return 2;
                break;

            case 0x3E:
                srlHl();
                return 4;
                break;

            case 0x3F:
                srlR(&(registers->a));
                return 2;
                break;

            
            default:
                break;
        }
        break;
    } 
        
    case 0xCC: { //CALL Z, nn
        bool condition = registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        callCIm(condition);
        if(condition){
            return 6;
        }
        return 3;
        break;      
    }

    case 0xCD: //CALL nn
        callIm();
        return 6;
        break;      
    
    case 0xCE: //ADC A, n
        adcImA();
        return 2;
        break;      
    
    case 0xCF: //RST 08h
        rst(0x0008);
        return 4;
        break;

    case 0xD0: { //RET NC
        bool condition = !registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        retC(condition);
        if(condition){
            return 5;
        }
        return 2;
        break;  
    }    
    
    case 0xD1: //POP DE
        popR(&(registers->de));
        return 3;
        break;
    
    case 0xD2: { //JP NC, nn
        bool condition = !registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        jpCIm(condition);
        if(condition){
            return 4;
        }
        return 3;
        break;
    }
    
    //0xD3 is empty
    
    case 0xD4: { //CALL NC, nn
        bool condition = !registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        callCIm(condition);
        if(condition){
            return 6;
        }
        return 3;
        break;
    }
    
    case 0xD5: //PUSH DE
        pushR(&(registers->de));
        return 4;
        break;
    
    case 0xD6: //SUB A, n
        subImA();
        return 2;
        break;
    
    case 0xD7: //RST 10h
        rst(0x0010);
        return 4;
        break;      
    
    case 0xD8:  { //RET C
        bool condition = registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        retC(condition);
        if(condition){
            return 5;
        }
        return 2;
        break;  
    }    

    case 0xD9: //RETI
        retI();
        return 4;
        break;      
    
    case 0xDA: { //JP C, nn
        bool condition = registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        jpCIm(condition);
        if(condition){
            return 4;
        }
        return 3;
        break;   
    }   
    
    //0xDB is empty     
    
    case 0xDC: { //CALL C, nn
        bool condition = registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        callCIm(condition);
        if(condition){
            return 6;
        }
        return 3;
        break;    
    }  
    
    //0xDD is empty
    
    case 0xDE: //SBC A, n
        sbcImA();
        return 2;
        break;      
    
    case 0xDF: //RST 18h
        rst(0x0018);
        return 4;
        break;

    case 0xE0: //LD (FF00+n), A
        loadAHIm();
        return 3;
        break;
    
    case 0xE1: //POP HL
        popR(&(registers->hl));
        return 3;
        break;
    
    case 0xE2: //LD (FF00+C), A
        loadAHC();
        return 2;
        break;
    
    //0xE3 and 0xE4 are empty
    
    case 0xE5: //PUSH HL
        pushR(&(registers->hl));
        return 4;
        break;
    
    case 0xE6: //AND A, n
        andAIm();
        return 2;
        break;
    
    case 0xE7: //RST 20h
        rst(0x0020);
        return 4;
        break;      
    
    case 0xE8: //ADD SP, n
        addSpE();
        return 4;
        break;      
    
    case 0xE9: //JP HL
        jpHl();
        return 1;
        break;      
    
    case 0xEA: //LD (nn), A
        loadAIm();
        return 4;
        break;      
    
    //0xEB, 0xEC and 0xED are empty         
    
    case 0xEE: //XOR A, n
        xorAIm();
        return 2;
        break;      
    
    case 0xEF: //RST 28h
        rst(0x0028);
        return 4;
        break;
    
    case 0xF0: //LD A, (FF00+n)
        loadHImA();
        return 3;
        break;
    
    case 0xF1: //POP AF
        popR(&(registers->af));
        registers->f &= 0xF0;
        return 3;
        break;
    
    case 0xF2: //LD A, (FF00+C)
        loadHCA();
        return 2;
        break;
    
    case 0xF3: //DI
        di();
        return 1;
        break;
    
    //0xF4 is empty
    
    case 0xF5: //PUSH AF
        pushR(&(registers->af));
        return 4;
        break;
    
    case 0xF6: //OR A, n
        orAIm();
        return 2;
        break;
    
    case 0xF7: //RST 30h
        rst(0x0030);
        return 4;
        break;      
    
    case 0xF8: //LD HL, SP+e
        loadSpEHl();
        return 3;
        break;      
    
    case 0xF9: //LD SP, HL
        loadHlSp();
        return 2;
        break;      
    
    case 0xFA: //LD A, nn
        loadImA();
        return 4;
        break;  
    
    case 0xFB: //EI
        ei();
        return 1;
        break;      
    
    //0xFC and 0xFD are empty   
    
    case 0xFE: //CP A, n
        cpAIm();
        return 2;
        break;      
    
    case 0xFF: //RST 38h
        rst(0x0038);
        return 4;
        break;
    }
    return 0;
}

// Basic Instructions
void Instructions::halt() { //????????????????????????????????
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
    uint16_t jumpAddress = readNext16Bit();
    if(condition){
        registers->sp--;
        memory->write(registers->sp, (registers->pc>>8) & 0xFF);  
        registers->sp--;
        memory->write(registers->sp, registers->pc & 0xFF);
        registers->pc = jumpAddress; 
    }
}

void Instructions::ret() {
    uint8_t lsb = memory->read(registers->sp);
    registers->sp++;
    uint8_t msb = memory->read(registers->sp);
    registers->sp++;
    registers->pc = (static_cast<uint16_t>(msb) << 8) | lsb;
}

void Instructions::retC(bool condition) {
    if(condition){
        uint8_t lsb = memory->read(registers->sp);
        registers->sp++;
        uint8_t msb = memory->read(registers->sp);
        registers->sp++;
        registers->pc = (static_cast<uint16_t>(msb) << 8) | lsb;
    }
}

void Instructions::retI() {
    interrupt->setIME(true);
    uint8_t lsb = memory->read(registers->sp);
    registers->sp++;
    uint8_t msb = memory->read(registers->sp);
    registers->sp++;
    registers->pc = (static_cast<uint16_t>(msb) << 8) | lsb;
}

void Instructions::rst(uint16_t address) {
    registers->sp--;
    memory->write(registers->sp, (registers->pc >>8) & 0xFF);
    registers->sp--;
    memory->write(registers->sp, registers->pc & 0xFF);
    registers->pc = address;
}

// Load 8 bit
void Instructions::loadRR8(uint8_t *dest, uint8_t *source) {
    *dest = *source;
}

void Instructions::loadImR8(uint8_t *dest) {
    *dest = readNext8Bit();
}

void Instructions::loadHlR(uint8_t *dest) {
    *dest = memory->read(registers->hl);
}

void Instructions::loadRHl(uint8_t *source) {
    memory->write(registers->hl, *source);
}

void Instructions::loadImHl() {
    memory->write(registers->hl, readNext8Bit());
}

void Instructions::loadRA(uint16_t *reg) {
    memory->write(*reg, registers->a);
}

void Instructions::loadAR(uint16_t *reg) {
    registers->a = memory->read(*reg);
}

void Instructions::loadImA() {
    uint16_t address = readNext16Bit();
    registers->a = memory->read(address);
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
    memory->write(0xFF00 + readNext8Bit(), registers->a);
}

void Instructions::loadHImA() {
    uint8_t value = readNext8Bit();
    registers->a = memory->read(0xFF00 + value);
}

void Instructions::loadAHlMinus() {
    memory->write(registers->hl, registers->a);
    registers->hl--;
}

void Instructions::loadHlMinusA() {
    registers->a = memory->read(registers->hl);
    registers->hl--;
}

void Instructions::loadAHlPlus() {
    memory->write(registers->hl, registers->a);
    registers->hl++;
}

void Instructions::loadHlPlusA() {
    registers->a = memory->read(registers->hl);
    registers->hl++;
}

// Load 16 bit
void Instructions::loadImR16(uint16_t *dest) {   
    *dest = readNext16Bit();
}

void Instructions::loadSpIm() {
    uint16_t immediate = readNext16Bit();
    memory->write(immediate, registers->sp & 0xFF);
    memory->write(immediate + 1, (registers->sp >> 8) & 0xFF);
}

void Instructions::loadHlSp() {
    registers->sp = registers->hl;
}

void Instructions::pushR(uint16_t *source) {
    registers->sp--;
    memory->write(registers->sp, (*source >> 8) & 0xFF);
    registers->sp--;
    memory->write(registers->sp, *source & 0xFF);
}

void Instructions::popR(uint16_t *dest) {
    uint8_t lsb = memory->read(registers->sp);
    registers->sp++;
    uint8_t msb = memory->read(registers->sp);
    registers->sp++;
    uint16_t res = (static_cast<uint16_t>(msb) <<8) | lsb;
    *dest = res;
}

void Instructions::loadSpEHl() {
    uint16_t sp = registers->sp;
    int8_t e = (int8_t) readNext8Bit();
    uint16_t value = sp + e;
    registers->hl = value;

    registers->setFlag(RegistersFlags::CARRY_FLAG, ((sp & 0xFF) + (uint8_t)e) > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, ((sp & 0x0F) + (e & 0x0F)) > 0x0F);
}

// Arithmetic and logical 8 bit
void Instructions::addRA(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result = a + *reg;
    registers->a = result;
    
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) + (*reg & 0xF) > 0xF);
}

void Instructions::addImA(){
    uint8_t value = readNext8Bit();
    uint8_t a = registers->a;
    uint16_t result = a + value;
    registers->a = result & 0xFF;
    
    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) + (value & 0xF) > 0xF);
    
}

void Instructions::addHlA() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result = a + hlValue;
    registers->a = result;
    
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) + (hlValue & 0xF) > 0xF);
}

void Instructions::adcRA(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    result = a + *reg + carry;

    registers->a = result;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) + (*reg & 0xF) + carry > 0xF);
}

void Instructions::adcHlA() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    
    result = a + hlValue + carry;
    registers->a = result;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) + (hlValue & 0xF) + carry > 0xF);
}

void Instructions::adcImA() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint16_t result;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    result = a + value + carry;
    registers->a = result & 0xFF;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) + (value & 0xF) + carry > 0xF);
}

void Instructions::subRA(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result = a - *reg;
    registers->a = result;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, *reg > a);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    //resto per i primi 4 bit. controllo se i primi 4 bit del registro a cui sommo sono maggiori di A
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (*reg & 0xF));
}

void Instructions::subHlA() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result = a - hlValue;
    registers->a = result;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, hlValue > a);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    //resto per i primi 4 bit. controllo se i primi 4 bit del registro a cui sommo sono maggiori di A
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (hlValue & 0xF));
}

void Instructions::subImA() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint16_t result = a - value;
    registers->a = result;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, value > a);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    //resto per i primi 4 bit. controllo se i primi 4 bit del registro a cui sommo sono maggiori di A
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (value & 0xF));
}

void Instructions::sbcRA(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;

    result = a - *reg - carry;
    registers->a = result;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, (uint16_t)(*reg) + carry > a);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (*reg & 0xF) + carry);
    
}

void Instructions::sbcHlA() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;

    result = a - hlValue - carry;
    registers->a = result;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, (uint16_t)(hlValue) + carry > a);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (hlValue & 0xF) + carry);
    
}

void Instructions::sbcImA() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint16_t result;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;

    result = a - value - carry;
    registers->a = result & 0xFF;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, (uint16_t)(value) + carry > a);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (value & 0xF) + carry);
    
}

void Instructions::cpAR(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result = a - *reg;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (*reg & 0xF));
}

void Instructions::cpAHl() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result = a - hlValue;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (hlValue & 0xF));
}

void Instructions::cpAIm() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint16_t result = a - value;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (value & 0xF));
}

void Instructions::incR8(uint8_t *reg) {
    uint8_t oldRegValue = *reg;
    (*reg)++;

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (oldRegValue & 0x0F) + 1 > 0x0F);

}

void Instructions::incHl() {
    uint8_t value = memory->read(registers->hl);
    uint8_t oldValue = value;
    value++;
    memory->write(registers->hl, value);

    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::ZERO_FLAG, value == 0);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (oldValue & 0x0F) + 1 > 0x0F);

}

void Instructions::decHl() {
    uint8_t value = memory->read(registers->hl);
    uint8_t oldValue = value;
    value--;
    memory->write(registers->hl, value);

    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    registers->setFlag(RegistersFlags::ZERO_FLAG, value == 0);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (oldValue & 0x0F) == 0);

}

void Instructions::decR8(uint8_t *reg) {
    uint8_t oldRegValue = *reg;
    (*reg)--;

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //half carry qui indica il prestito dal quarto bit. Il prestito avviene solo se i primi 4 bit sono a 0
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (oldRegValue & 0x0F) == 0);

}

void Instructions::andAR(uint8_t *reg) {
    uint8_t a = registers->a & *reg;
    registers->a = a;

    registers->setFlag(RegistersFlags::ZERO_FLAG, a == 0); //true se 0 altrimenti false
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::andAHl() {
    uint8_t res = registers->a & memory->read(registers->hl);
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0); //true se 0 altrimenti false
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::andAIm() {
    uint8_t a = registers->a;
    uint8_t res = a & readNext8Bit();
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0); //true se 0 altrimenti false
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::orAR(uint8_t *reg) {
    uint8_t res = registers->a | *reg;
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

}

void Instructions::orAHl() {
    uint8_t res = registers->a | memory->read(registers->hl);
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::orAIm() {
    uint8_t res = registers->a | readNext8Bit();
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::xorAR(uint8_t *reg) {
    uint8_t a = registers->a;
    uint8_t res = a ^ *reg;
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::xorAHl() {
    uint8_t a = registers->a;
    uint8_t res = a ^ memory->read(registers->hl);
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::xorAIm() {
    uint8_t a = registers->a;
    uint8_t res = a ^ readNext8Bit();
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::ccf() {
    registers->setFlag(RegistersFlags::CARRY_FLAG, !registers->isFlagSet(RegistersFlags::CARRY_FLAG));
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
}

void Instructions::scf() {
    registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
}


void Instructions::daA() { 
    uint8_t adj = 0; 
    if(registers->isFlagSet(RegistersFlags::SUBTRACTION_FLAG)){
        if(registers->isFlagSet(RegistersFlags::HALF_CARRY_FLAG)){
            adj += 0x6;
        }
        if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
            adj+= 0x60;
        }
        registers->a -= adj;
    }else{
        if(registers->isFlagSet(RegistersFlags::HALF_CARRY_FLAG) || (registers->a & 0xF) > 0x9){
            adj += 0x6;
        }
        if (registers->isFlagSet(RegistersFlags::CARRY_FLAG) || registers->a > 0x99){
            adj += 0x60;
            registers->setFlag(RegistersFlags::CARRY_FLAG, true); 
        }
        registers->a += adj;
    }

    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::ZERO_FLAG, registers->a == 0);
}

void Instructions::cpA() {
    registers->a = ~registers->a;
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
}

// Arithmetic and logical 16 bit
void Instructions::incR16(uint16_t *reg) {
    (*reg)++;
}

void Instructions::decR16(uint16_t *reg) {
    (*reg)--;
}

void Instructions::addHlR(uint16_t *reg) {
    uint16_t hl = registers->hl;
    uint32_t result = hl + *reg;
    registers->hl = result;

    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (hl & 0x0FFF) + (*reg & 0x0FFF) > 0x0FFF);
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFFFF);

}

void Instructions::addSpE() {
    uint16_t sp = registers->sp;
    int8_t e = (int8_t) readNext8Bit();
    uint16_t result = sp + e;
    registers->sp = result;

    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, ((sp & 0x0F) + (e & 0x0F)) > 0x0F);
    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    //overflow from bit 7
    registers->setFlag(RegistersFlags::CARRY_FLAG, ((sp & 0xFF) + (uint8_t)e) > 0xFF);
}

// Bit operations

void Instructions::rla() {
    uint8_t a = registers->a;
    uint8_t msb = (a & 0x80) >> 7;
    
    registers->a = a << 1 | registers->isFlagSet(RegistersFlags::CARRY_FLAG);

    registers->setFlag(RegistersFlags::CARRY_FLAG, msb == 1);
    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
}

void Instructions::rra() {
    uint8_t oldValue = registers->a;
    uint8_t lsb = oldValue & 0x01;
    registers->a = oldValue >> 1 | (registers->isFlagSet(RegistersFlags::CARRY_FLAG) << 7);

    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb == 1);
    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
}

void Instructions::rlcR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    *reg = oldValue << 1 | bit7;

    registers->setFlag(RegistersFlags::ZERO_FLAG, (*reg) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7==1);
}

void Instructions::rlca() {
    uint8_t oldValue = registers->a;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    registers->a = oldValue << 1 | bit7;

    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7==1);
}

void Instructions::rlcHl() {
    uint8_t oldValue = memory->read(registers->hl);
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    uint8_t result = oldValue << 1 | bit7;
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7==1);
}

void Instructions::rrcR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t lsb = oldValue & 0x01;
    *reg = oldValue >> 1 | (lsb << 7);

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb==1);
}

void Instructions::rrca() {
    uint8_t oldValue = registers->a;
    uint8_t lsb = oldValue & 0x01;
    registers->a = oldValue >> 1 | (lsb << 7);

    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb==1);
}

void Instructions::rrcHl() {
    uint8_t oldValue = memory->read(registers->hl);
    uint8_t lsb = oldValue & 0x01;
    registers->a = oldValue >> 1 | (lsb << 7);

    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb==1);
}

void Instructions::rlR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    *reg = oldValue << 1 | carry;

    registers->setFlag(RegistersFlags::ZERO_FLAG, (*reg) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7);
}

void Instructions::rlHl() {
    uint8_t oldValue = memory->read(registers->hl);
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    uint8_t result = oldValue << 1 | carry;
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7);
}

void Instructions::rrR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t lsb = oldValue & 0x01;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    *reg = oldValue >> 1 | (carry << 7);

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);
}

void Instructions::rrHl() {
    uint8_t oldValue = memory->read(registers->hl);
    uint8_t lsb = oldValue & 0x01;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    uint8_t result = oldValue >> 1 | (carry << 7);
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);
}

void Instructions::slaR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    *reg = oldValue << 1;

    registers->setFlag(RegistersFlags::ZERO_FLAG, (*reg) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7);
}

void Instructions::slaHl() {
    uint8_t oldValue = memory->read(registers->hl);
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    uint8_t result = oldValue << 1;
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7);
}

void Instructions::sraR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t lsb = oldValue & 0x01;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    *reg = (oldValue >> 1) | bit7;

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);
}

void Instructions::sraHl() {
    uint8_t oldValue = memory->read(registers->hl);
    uint8_t lsb = oldValue & 0x01;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    uint8_t result = (oldValue >> 1) | bit7;
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);
}

void Instructions::swapR(uint8_t *reg) {
     uint8_t oldValue = *reg;
    *reg = (((oldValue & 0xF0) >> 4) | ((oldValue & 0x0F) << 4));

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::swapHl() {
    uint8_t oldValue = memory->read(registers->hl);
    uint8_t result = (((oldValue & 0xF0) >> 4) | ((oldValue & 0x0F) << 4));
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::srlR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t lsb = oldValue & 0x01;
    *reg = oldValue >> 1;

    registers->setFlag(RegistersFlags::ZERO_FLAG, (*reg) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);
}

void Instructions::srlHl() {
    uint8_t oldValue = memory->read(registers->hl);
    uint8_t lsb = oldValue & 0x01;
    uint8_t result = oldValue >> 1;
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);
}

void Instructions::bitBR(uint8_t *reg) {}
void Instructions::bitBHl() {}
void Instructions::resBR(uint8_t *reg) {}
void Instructions::resBHl() {}
void Instructions::setBR(uint8_t *reg) {}
void Instructions::setBHl() {}
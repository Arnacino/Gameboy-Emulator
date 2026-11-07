#include "instructions.h"
#include "../memory/registers.h"
#include "../memory/memory.h"
#include "interrupt.h"

Instructions::Instructions(Registers *registers, Memory *memory, Interrupt *interrupt) {
    this->registers = registers;
    this->memory = memory;
    this->interrupt = interrupt;
}

uint8_t Instructions::readNext8Bit(){
    uint8_t immediate = memory->read(registers->pc);
    registers->pc++;
    return immediate;
}

bool Instructions::execute(uint8_t instruction, int instructionCycle){

    bool condition = false;

    switch (instruction){

    case 0x00: //NOP
        return true;

    case 0x01: //LD BC, nn
        return loadImR16(&(registers->bc), instructionCycle);

    case 0x02: //LD BC, A
        return loadRA(&(registers->bc));

    case 0x03: // INC BC
        return incR16(&(registers->bc), instructionCycle);

    case 0x04: //INC B
        return incR8(&(registers->b));;
    
    case 0x05: //DEC B
        return decR8(&(registers->b));
        
    case 0x06: // LD B, n
        return loadImR8(&(registers->b));
        
    case 0x07: //RLCA (sarebbe come uno shift left però l'ultimo bit diventa il primo)
        return rlca();
        
    case 0x08: //LD SP, nn
        return loadSpIm(instructionCycle);
        
    case 0x09: //ADD HL, BC
        return addHlR(&(registers->bc));
        
    case 0x0A: //LD A, BC
        return loadAR(&(registers->bc));
        
    case 0x0B: //DEC BC
        return decR16(&(registers->bc), instructionCycle);
        
    case 0x0C: //INC C
        return incR8(&(registers->c));
        
    case 0x0D: //DEC C
        return decR8(&(registers->c));
        
    case 0x0E: //LD C, u8
        return loadImR8(&(registers->c));
        
    case 0x0F: //RRCA
        return rrca();
        
    case 0x10: //STOP
        return stop();
        
    case 0x11: // LD DE, nn
        return loadImR16(&(registers->de), instructionCycle);

    case 0x12: // LD DE, A
        return loadRA(&(registers->de));

    case 0x13: // INC DE
        return incR16(&(registers->de), instructionCycle);

    case 0x14: // INC D
        return incR8(&(registers->d));

    case 0x15: // DEC D
        return decR8(&(registers->d));

    case 0x16: // LD D, n
        return loadImR8(&(registers->d));

    case 0x17: // RLA
        return rla();

    case 0x18: // JR n 
        return jrIm(instructionCycle);

    case 0x19: // ADD HL, DE
        return addHlR(&(registers->de));

    case 0x1A: // LD A, DE
        return loadAR(&(registers->de));

    case 0x1B: // DEC DE
        return decR16(&(registers->de), instructionCycle);

    case 0x1C: // INC E
        return incR8(&(registers->e));

    case 0x1D: // DEC E
        return decR8(&(registers->e));

    case 0x1E: // LD E, n
        return loadImR8(&(registers->e));

    case 0x1F: // RRA
        return rra();

    case 0x20:  // JR NZ, n
        condition = !registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        return jrCIm(condition, instructionCycle);
        
    case 0x21: // LD HL, nn
        return loadImR16(&(registers->hl), instructionCycle);
    
    case 0x22: // LD HL+, A
        return loadAHlPlus();
    
    case 0x23: // INC HL
        return incR16(&(registers->hl), instructionCycle);
        
    case 0x24: //INC H
        return incR8(&(registers->h));
        
    case 0x25: //DEC H
        return decR8(&(registers->h));
        
    case 0x26: //LD H, n
        return loadImR8(&(registers->h));
        
    case 0x27: //DAA
        return daA();
        
    case 0x28:   //JR Z, n
        condition = registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        return jrCIm(condition, instructionCycle);

    case 0x29: //ADD HL, HL 
        return addHlR(&(registers->hl));
        
    case 0x2A: //LD A, HL+
        return loadHlPlusA();
        
    case 0x2B: //DEC HL
        return decR16(&(registers->hl), instructionCycle);
        
    case 0x2C: //INC L
        return incR8(&(registers->l));
        
    case 0x2D: //DEC L
        return decR8(&(registers->l));
        
    case 0x2E: //LD L, n
        return loadImR8(&(registers->l));
        
    case 0x2F: //CPL
        return cpA();
        
    case 0x30: //JR NC, n
        condition = !registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        return jrCIm(condition, instructionCycle);
        
    case 0x31: // LD SP, nn
        return loadImR16(&(registers->sp), instructionCycle);

    case 0x32: //LD HL-, A
        return loadAHlMinus();
        
    case 0x33: // INC SP
        return incR16(&(registers->sp), instructionCycle);

    case 0x34: //INC (HL)
        return incHl(instructionCycle);
        
    case 0x35: //DEC (HL)
        return decHl(instructionCycle);
        
    case 0x36: //LD (HL), n
        return loadImHl(instructionCycle);
        
    case 0x37: //SCF
        return scf();
        
    case 0x38: //JR C, n
        condition = registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        return jrCIm(condition, instructionCycle);

    case 0x39: //ADD HL, SP
        return addHlR(&(registers->sp));
        
    case 0x3A: //LD A, (HL-)
        return loadHlMinusA();
        
    case 0x3B: //DEC SP 
        return decR16(&(registers->sp), instructionCycle);
        
    case 0x3C: //INC A
        return incR8(&(registers->a));
        
    case 0x3D: //DEC A
        return decR8(&(registers->a));
        
    case 0x3E: //LD A, n
        return loadImR8(&(registers->a));
        
    case 0x3F: //CCF
        return ccf();
        
    case 0x40: //LD B, B
        return loadRR8(&(registers->b), &(registers->b));
    
    case 0x41: //LD B, C
        return loadRR8(&(registers->b), &(registers->c));
    
    case 0x42: //LD B, D
        return loadRR8(&(registers->b), &(registers->d));
    
    case 0x43: //LD B, E
        return loadRR8(&(registers->b), &(registers->e));
    
    case 0x44: //LD B, H
        return loadRR8(&(registers->b), &(registers->h));
    
    case 0x45: //LD B, L
        return loadRR8(&(registers->b), &(registers->l));
    
    case 0x46: //LD B, (HL)
        return loadHlR(&(registers->b));
    
    case 0x47: //LD B, A
        return loadRR8(&(registers->b), &(registers->a));
    
    case 0x48: //LD C, B
        return loadRR8(&(registers->c), &(registers->b));
    
    case 0x49: //LD C, C
        return loadRR8(&(registers->c), &(registers->c));
    
    case 0x4A: //LD C, D
        return loadRR8(&(registers->c), &(registers->d));
    
    case 0x4B: //LD C, E
        return loadRR8(&(registers->c), &(registers->e));
    
    case 0x4C: //LD C, H    
        return loadRR8(&(registers->c), &(registers->h));
    
    case 0x4D: //LD C, L
        return loadRR8(&(registers->c), &(registers->l));
    
    case 0x4E: //LD C, (hl)
        return loadHlR(&(registers->c));
    
    case 0x4F: //LD C, A
        return loadRR8(&(registers->c), &(registers->a));
        
    case 0x50: //LD D, B
        return loadRR8(&(registers->d), &(registers->b));
    
    case 0x51: //LD D, C
        return loadRR8(&(registers->d), &(registers->c));
    
    case 0x52: //LD D, D
        return loadRR8(&(registers->d), &(registers->d));
    
    case 0x53: //LD D, E
        return loadRR8(&(registers->d), &(registers->e));
    
    case 0x54: //LD D, H    
        return loadRR8(&(registers->d), &(registers->h));
    
    case 0x55: //LD D, L
        return loadRR8(&(registers->d), &(registers->l));
    
    case 0x56: //LD D, (hl)
        return loadHlR(&(registers->d));
    
    case 0x57: //LD D, A
        return loadRR8(&(registers->d), &(registers->a));
            
    case 0x58: //LD E, B
        return loadRR8(&(registers->e), &(registers->b));
    
    case 0x59: //LD E, C
        return loadRR8(&(registers->e), &(registers->c));
    
    case 0x5A: //LD E, D
        return loadRR8(&(registers->e), &(registers->d));
    
    case 0x5B: //LD E, E
        return loadRR8(&(registers->e), &(registers->e));
    
    case 0x5C: //LD E, H    
        return loadRR8(&(registers->e), &(registers->h));
    
    case 0x5D: //LD E, L
        return loadRR8(&(registers->e), &(registers->l));
    
    case 0x5E: //LD E, (hl)
        return loadHlR(&(registers->e));
    
    case 0x5F: //LD E, A
        return loadRR8(&(registers->e), &(registers->a));
    
    case 0x60: //LD H, B
        return loadRR8(&(registers->h), &(registers->b));
    
    case 0x61: //LD H, C
        return loadRR8(&(registers->h), &(registers->c));
    
    case 0x62: //LD H, D
        return loadRR8(&(registers->h), &(registers->d));
    
    case 0x63: //LD H, E
        return loadRR8(&(registers->h), &(registers->e));
    
    case 0x64: //LD H, H    
        return loadRR8(&(registers->h), &(registers->h));
    
    case 0x65: //LD H, L
        return loadRR8(&(registers->h), &(registers->l));
    
    case 0x66: //LD H, (hl)
        return loadHlR(&(registers->h));
    
    case 0x67: //LD H, A
        return loadRR8(&(registers->h), &(registers->a));
    
    case 0x68: //LD L, B
        return loadRR8(&(registers->l), &(registers->b));
    
    case 0x69: //LD L, C
        return loadRR8(&(registers->l), &(registers->c));
    
    case 0x6A: //LD L, D
        return loadRR8(&(registers->l), &(registers->d));
    
    case 0x6B: //LD L, E
        return loadRR8(&(registers->l), &(registers->e));
    
    case 0x6C: //LD L, H
        return loadRR8(&(registers->l), &(registers->h));
    
    case 0x6D: //LD L, L
        return loadRR8(&(registers->l), &(registers->l));
    
    case 0x6E: //LD L, (HL)
        return loadHlR(&(registers->l));
    
    case 0x6F: //LD L, A
        return loadRR8(&(registers->l), &(registers->a));
    
    case 0x70: //LD (HL), B
        return loadRHl(&(registers->b));
    
    case 0x71: //LD (HL), C
        return loadRHl(&(registers->c));
    
    case 0x72: //LD (HL), D
        return loadRHl(&(registers->d));
    
    case 0x73: //LD (HL), E
        return loadRHl(&(registers->e));
    
    case 0x74: //LD (HL), H    
        return loadRHl(&(registers->h));
    
    case 0x75: //LD (HL), L
        return loadRHl(&(registers->l));
    
    case 0x76: //HALT
        return halt();
    
    case 0x77: //LD (HL), A
        return loadRHl(&(registers->a));
    
    case 0x78: //LD A, B
        return loadRR8(&(registers->a), &(registers->b));
    
    case 0x79: //LD A, C
        return loadRR8(&(registers->a), &(registers->c));
    
    case 0x7A: //LD A, D
        return loadRR8(&(registers->a), &(registers->d));
    
    case 0x7B: //LD A, E
        return loadRR8(&(registers->a), &(registers->e));
    
    case 0x7C: //LD A, H
        return loadRR8(&(registers->a), &(registers->h));
    
    case 0x7D: //LD A, L
        return loadRR8(&(registers->a), &(registers->l));
    
    case 0x7E: //LD A, (HL)
        return loadHlR(&(registers->a));
    
    case 0x7F: //LD A, A
        return loadRR8(&(registers->a), &(registers->a));

    case 0x80: //ADD A, B
        return addRA(&(registers->b));
    
    case 0x81: //ADD A, C
        return addRA(&(registers->c));
    
    case 0x82: //ADD A, D
        return addRA(&(registers->d));
    
    case 0x83: //ADD A, E
        return addRA(&(registers->e));
    
    case 0x84: //ADD A, H
        return addRA(&(registers->h));
    
    case 0x85: //ADD A, L
        return addRA(&(registers->l));
    
    case 0x86: //ADD A, (HL)
        return addHlA();
    
    case 0x87: //ADD A, A
        return addRA(&(registers->a));  
    
    case 0x88: //ADC A, B
        return adcRA(&(registers->b));
    
    case 0x89: //ADC A, C
        return adcRA(&(registers->c));
    
    case 0x8A: //ADC A, D
        return adcRA(&(registers->d));
    
    case 0x8B: //ADC A, E
        return adcRA(&(registers->e));
    
    case 0x8C: //ADC A, H
        return adcRA(&(registers->h));
    
    case 0x8D: //ADC A, L
        return adcRA(&(registers->l));
    
    case 0x8E: //ADC A, (HL)
        return adcHlA();
    
    case 0x8F: //ADC A, A
        return adcRA(&(registers->a));
    
    case 0x90: //SUB A, B
        return subRA(&(registers->b));
    
    case 0x91: //SUB A, C
        return subRA(&(registers->c));
    
    case 0x92: //SUB A, D
        return subRA(&(registers->d));
    
    case 0x93: //SUB A, E
        return subRA(&(registers->e));
    
    case 0x94: //SUB A, H
        return subRA(&(registers->h));
    
    case 0x95: //SUB A, L
        return subRA(&(registers->l));
    
    case 0x96: //SUB A, (HL)
        return subHlA();
    
    case 0x97: //SUB A, A
        return subRA(&(registers->a));
    
    case 0x98: //SBC A, B
        return sbcRA(&(registers->b));
    
    case 0x99: //SBC A, C
        return sbcRA(&(registers->c));
    
    case 0x9A: //SBC A, D
        return sbcRA(&(registers->d));
    
    case 0x9B: //SBC A, E
        return sbcRA(&(registers->e));
    
    case 0x9C: //SBC A, H
        return sbcRA(&(registers->h));
    
    case 0x9D: //SBC A, L
        return sbcRA(&(registers->l));
    
    case 0x9E: //SBC A, (HL)
        return sbcHlA();
    
    case 0x9F: //SBC A, A
        return sbcRA(&(registers->a));

    case 0xA0: //AND A, B
        return andAR(&(registers->b));
    
    case 0xA1: //AND A, C
        return andAR(&(registers->c));
    
    case 0xA2: //AND A, D
        return andAR(&(registers->d));
    
    case 0xA3: //AND A, E
        return andAR(&(registers->e));
    
    case 0xA4: //AND A, H
        return andAR(&(registers->h));
    
    case 0xA5: //AND A, L
        return andAR(&(registers->l));
    
    case 0xA6: //AND A, (HL)
        return andAHl();
    
    case 0xA7: //AND A, A
        return andAR(&(registers->a));
    
    case 0xA8: //XOR A, B
        return xorAR(&(registers->b));
    
    case 0xA9: //XOR A, C
        return xorAR(&(registers->c));

    case 0xAA: //XOR A, D
        return xorAR(&(registers->d));
    
    case 0xAB: //XOR A, E
        return xorAR(&(registers->e));
    
    case 0xAC: //XOR A, H
        return xorAR(&(registers->h));
    
    case 0xAD: //XOR A, L
        return xorAR(&(registers->l));
    
    case 0xAE: //XOR A, (HL)
        return xorAHl();
              
    case 0xAF: //XOR A, A
        return xorAR(&(registers->a));

    case 0xB0: //OR A, B
        return orAR(&(registers->b));
        
    case 0xB1: //OR A, C
        return orAR(&(registers->c));
    
    case 0xB2: //OR A, D
        return orAR(&(registers->d));
        
    case 0xB3: //OR A, E
        return orAR(&(registers->e));
        
    case 0xB4: //OR A, H
        return orAR(&(registers->h));
        
    case 0xB5: //OR A, L
        return orAR(&(registers->l));
        
    case 0xB6: //OR A, (HL)
        return orAHl();
        
    case 0xB7: //OR A, A
        return orAR(&(registers->a));
        
    case 0xB8: //CP A, B
        return cpAR(&(registers->b));
        
    case 0xB9: //CP A, C
        return cpAR(&(registers->c));

    case 0xBA: //CP A, D
        return cpAR(&(registers->d));
        
    case 0xBB: //CP A, E
        return cpAR(&(registers->e));
        
    case 0xBC: //CP A, H
        return cpAR(&(registers->h));
        
   case 0xBD: //CP A, L
       return cpAR(&(registers->l));
        
    case 0xBE: //CP A, (HL)
        return cpAHl();
        
    case 0xBF: //CP A, A
        return cpAR(&(registers->a));

    case 0xC0: //RET NZ
        condition = !registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        return retC(condition, instructionCycle);
    
    case 0xC1: //POP BC
        return popR(&(registers->bc), instructionCycle);
    
    case 0xC2: //JP NZ, nn
        condition = !registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        return jpCIm(condition, instructionCycle);

    case 0xC3: //JP nn
        return jpIm(instructionCycle);
    
    case 0xC4:  //CALL NZ, nn
        condition = !registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        return callCIm(condition, instructionCycle);

    case 0xC5: //PUSH BC
        return pushR(&(registers->bc), instructionCycle);
    
    case 0xC6: //ADD A, n
        return addImA();
        
    case 0xC7: //RST 00h
        return rst(0x0000, instructionCycle);
    
    case 0xC8: //RET Z
        condition = registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        return retC(condition, instructionCycle);
    
    case 0xC9: //RET
        return ret(instructionCycle);
    
    case 0xCA: //JP Z, nn
        condition = registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        return jpCIm(condition, instructionCycle);
    
    case 0xCB: { //PREFIX CB 
        if (instructionCycle == 1) {
            cbOpcode = readNext8Bit();
        }
        switch(cbOpcode) {
            case 0x00:
                return rlcR(&(registers->b));
            
            case 0x01:
                return rlcR(&(registers->c));
            
            case 0x02:
                return rlcR(&(registers->d));
            
            case 0x03:
                return rlcR(&(registers->e));
            
            case 0x04:
                return rlcR(&(registers->h));
            
            case 0x05:
                return rlcR(&(registers->l));

            case 0x06:
                return rlcHl(instructionCycle);

            case 0x07:
                return rlcR(&(registers->a));

            case 0x08:
                return rrcR(&(registers->b));

            case 0x09:
                return rrcR(&(registers->c));
                
            case 0x0A:
                return rrcR(&(registers->d));
                
            case 0x0B:
                return rrcR(&(registers->e));

            case 0x0C:
                return rrcR(&(registers->h));
                
            case 0x0D:
                return rrcR(&(registers->l));
                
            case 0x0E:
                return rrcHl(instructionCycle);

            case 0x0F:
                return rrcR(&(registers->a));

            case 0x10:
                return rlR(&(registers->b));
                
            case 0x11:
                return rlR(&(registers->c));

            case 0x12:
                return rlR(&(registers->d));

            case 0x13:
                return rlR(&(registers->e));

            case 0x14:
                return rlR(&(registers->h));

            case 0x15:
                return rlR(&(registers->l));

            case 0x16:
                return rlHl(instructionCycle);

            case 0x17:
                return rlR(&(registers->a));

            case 0x18:
                return rrR(&(registers->b));

            case 0x19:
                return rrR(&(registers->c));

            case 0x1A:
                return rrR(&(registers->d));

            case 0x1B:
                return rrR(&(registers->e));

            case 0x1C:
                return rrR(&(registers->h));

            case 0x1D:
                return rrR(&(registers->l));

            case 0x1E:
                return rrHl(instructionCycle);

            case 0x1F:
                return rrR(&(registers->a));

            case 0x20:
                return slaR(&(registers->b));

            case 0x21:
                return slaR(&(registers->c));

            case 0x22:
                return slaR(&(registers->d));

            case 0x23:
                return slaR(&(registers->e));

            case 0x24:
                return slaR(&(registers->h));

            case 0x25:
                return slaR(&(registers->l));

            case 0x26:
                return slaHl(instructionCycle);

            case 0x27:
                return slaR(&(registers->a));

            case 0x28:
                return sraR(&(registers->b));

            case 0x29:
                return sraR(&(registers->c));
                
            case 0x2A:
                return sraR(&(registers->d));
                
            case 0x2B:
                return sraR(&(registers->e));

            case 0x2C:
                return sraR(&(registers->h));
                
            case 0x2D:
                return sraR(&(registers->l));

            case 0x2E:
                return sraHl(instructionCycle);

            case 0x2F:
                return sraR(&(registers->a));
                
            case 0x30:
                return swapR(&(registers->b));
                
            case 0x31:
                return swapR(&(registers->c));

            case 0x32:
                return swapR(&(registers->d));

            case 0x33:
                return swapR(&(registers->e));

            case 0x34:
                return swapR(&(registers->h));

            case 0x35:
                return swapR(&(registers->l));

            case 0x36:
                return swapHl(instructionCycle);

            case 0x37:
                return swapR(&(registers->a));

            case 0x38:
                return srlR(&(registers->b));
                
            case 0x39:
                return srlR(&(registers->c));
                
            case 0x3A:
                return srlR(&(registers->d));

            case 0x3B:
                return srlR(&(registers->e));

            case 0x3C:
                return srlR(&(registers->h));

            case 0x3D:
                return srlR(&(registers->l));

            case 0x3E:
                return srlHl(instructionCycle);

            case 0x3F:
                return srlR(&(registers->a));

            case 0x40:
                return bitBR(0, &(registers->b));
            
            case 0x41:
                return bitBR(0, &(registers->c));
            
            case 0x42:
                return bitBR(0, &(registers->d));
            
            case 0x43:
                return bitBR(0, &(registers->e));
            
            case 0x44:
                return bitBR(0, &(registers->h));
            
            case 0x45:
                return bitBR(0, &(registers->l));

            case 0x46:
                return bitBHl(0, instructionCycle);

            case 0x47:
                return bitBR(0, &(registers->a));

            case 0x48:
                return bitBR(1, &(registers->b));

            case 0x49:
                return bitBR(1, &(registers->c));

            case 0x4A:
                return bitBR(1, &(registers->d));

            case 0x4B:
                return bitBR(1, &(registers->e));

            case 0x4C:
                return bitBR(1, &(registers->h));

            case 0x4D:
                return bitBR(1, &(registers->l));

            case 0x4E:
                return bitBHl(1, instructionCycle);

            case 0x4F:
                return bitBR(1, &(registers->a));

            case 0x50:
                return bitBR(2, &(registers->b));
            
            case 0x51:
                return bitBR(2, &(registers->c));
            
            case 0x52:
                return bitBR(2, &(registers->d));
            
            case 0x53:
                return bitBR(2, &(registers->e));
            
            case 0x54:
                return bitBR(2, &(registers->h));
            
            case 0x55:
                return bitBR(2, &(registers->l));

            case 0x56:
                return bitBHl(2, instructionCycle);

            case 0x57:
                return bitBR(2, &(registers->a));

            case 0x58:
                return bitBR(3, &(registers->b));

            case 0x59:
                return bitBR(3, &(registers->c));

            case 0x5A:
                return bitBR(3, &(registers->d));

            case 0x5B:
                return bitBR(3, &(registers->e));

            case 0x5C:
                return bitBR(3, &(registers->h));

            case 0x5D:
                return bitBR(3, &(registers->l));

            case 0x5E:
                return bitBHl(3, instructionCycle);

            case 0x5F:
                return bitBR(3, &(registers->a));

            case 0x60:
                return bitBR(4, &(registers->b));

            case 0x61:
                return bitBR(4, &(registers->c));

            case 0x62:
                return bitBR(4, &(registers->d));

            case 0x63:
                return bitBR(4, &(registers->e));

            case 0x64:
                return bitBR(4, &(registers->h));

            case 0x65:
                return bitBR(4, &(registers->l));

            case 0x66:
                return bitBHl(4, instructionCycle);

            case 0x67:
                return bitBR(4, &(registers->a));

            case 0x68:
                return bitBR(5, &(registers->b));

            case 0x69:
                return bitBR(5, &(registers->c));

            case 0x6A:
                return bitBR(5, &(registers->d));

            case 0x6B:
                return bitBR(5, &(registers->e));

            case 0x6C:
                return bitBR(5, &(registers->h));

            case 0x6D:
                return bitBR(5, &(registers->l));

            case 0x6E:
                return bitBHl(5, instructionCycle);

            case 0x6F:
                return bitBR(5, &(registers->a));

            case 0x70:
                return bitBR(6, &(registers->b));

            case 0x71:
                return bitBR(6, &(registers->c));

            case 0x72:
                return bitBR(6, &(registers->d));

            case 0x73:
                return bitBR(6, &(registers->e));

            case 0x74:
                return bitBR(6, &(registers->h));

            case 0x75:
                return bitBR(6, &(registers->l));

            case 0x76:
                return bitBHl(6, instructionCycle);

            case 0x77:
                return bitBR(6, &(registers->a));

            case 0x78:
                return bitBR(7, &(registers->b));

            case 0x79:
                return bitBR(7, &(registers->c));

            case 0x7A:
                return bitBR(7, &(registers->d));

            case 0x7B:
                return bitBR(7, &(registers->e));

            case 0x7C:
                return bitBR(7, &(registers->h));

            case 0x7D:
                return bitBR(7, &(registers->l));

            case 0x7E:
                return bitBHl(7, instructionCycle);

            case 0x7F:
                return bitBR(7, &(registers->a));

            case 0x80:
                return resBR(0, &(registers->b));
            
            case 0x81:
                return resBR(0, &(registers->c));
            
            case 0x82:
                return resBR(0, &(registers->d));
            
            case 0x83:
                return resBR(0, &(registers->e));
                
            case 0x84:
                return resBR(0, &(registers->h));
                
            case 0x85:
                return resBR(0, &(registers->l));

            case 0x86:
                return resBHl(0, instructionCycle);
                
            case 0x87:
                return resBR(0, &(registers->a));
                
            case 0x88:
                return resBR(1, &(registers->b));

            case 0x89:
                return resBR(1, &(registers->c));

            case 0x8A:
                return resBR(1, &(registers->d));

            case 0x8B:
                return resBR(1, &(registers->e));

            case 0x8C:
                return resBR(1, &(registers->h));

            case 0x8D:
                return resBR(1, &(registers->l));
                
            case 0x8E:
                return resBHl(1, instructionCycle);

            case 0x8F:
                return resBR(1, &(registers->a));

            case 0x90:
                return resBR(2, &(registers->b));

            case 0x91:
                return resBR(2, &(registers->c));

            case 0x92:
                return resBR(2, &(registers->d));
                
            case 0x93:
                return resBR(2, &(registers->e));

            case 0x94:
                return resBR(2, &(registers->h));

            case 0x95:
                return resBR(2, &(registers->l));
                
            case 0x96:
                return resBHl(2, instructionCycle);

            case 0x97:
                return resBR(2, &(registers->a));
                
            case 0x98:
                return resBR(3, &(registers->b));
                
            case 0x99:
                return resBR(3, &(registers->c));
                
            case 0x9A:
                return resBR(3, &(registers->d));
                
            case 0x9B:
                return resBR(3, &(registers->e));
                
            case 0x9C:
                return resBR(3, &(registers->h));

            case 0x9D:
                return resBR(3, &(registers->l));

            case 0x9E:
                return resBHl(3, instructionCycle);

            case 0x9F:
                return resBR(3, &(registers->a));
                
            case 0xA0:
                return resBR(4, &(registers->b));
                
            case 0xA1:
                return resBR(4, &(registers->c));
                
            case 0xA2:
                return resBR(4, &(registers->d));
                
            case 0xA3:
                return resBR(4, &(registers->e));
                
            case 0xA4:
                return resBR(4, &(registers->h));
                
            case 0xA5:
                return resBR(4, &(registers->l));

            case 0xA6:
                return resBHl(4, instructionCycle);

            case 0xA7:
                return resBR(4, &(registers->a));

            case 0xA8:
                return resBR(5, &(registers->b));
                
            case 0xA9:
                return resBR(5, &(registers->c));
                
            case 0xAA:
                return resBR(5, &(registers->d));

            case 0xAB:
                return resBR(5, &(registers->e));

            case 0xAC:
                return resBR(5, &(registers->h));

            case 0xAD:
                return resBR(5, &(registers->l));
                
            case 0xAE:
                return resBHl(5, instructionCycle);
                
            case 0xAF:
                return resBR(5, &(registers->a));

            case 0xB0:
                return resBR(6, &(registers->b));

            case 0xB1:
                return resBR(6, &(registers->c));

            case 0xB2:
                return resBR(6, &(registers->d));

            case 0xB3:
                return resBR(6, &(registers->e));

            case 0xB4:
                return resBR(6, &(registers->h));

            case 0xB5:
                return resBR(6, &(registers->l));

            case 0xB6:
                return resBHl(6, instructionCycle);

            case 0xB7:
                return resBR(6, &(registers->a));

            case 0xB8:
                return resBR(7, &(registers->b));

            case 0xB9:
                return resBR(7, &(registers->c));
                
            case 0xBA:
                return resBR(7, &(registers->d));

            case 0xBB:
                return resBR(7, &(registers->e));

            case 0xBC:
                return resBR(7, &(registers->h));

            case 0xBD:
                return resBR(7, &(registers->l));
                
            case 0xBE:
                return resBHl(7, instructionCycle);
                
            case 0xBF:
                return resBR(7, &(registers->a));
                
            case 0xC0:
                return setBR(0, &(registers->b));

            case 0xC1:
                return setBR(0, &(registers->c));
                
            case 0xC2:
                return setBR(0, &(registers->d));

            case 0xC3:
                return setBR(0, &(registers->e));
                
            case 0xC4:
                return setBR(0, &(registers->h));
                
            case 0xC5:
                return setBR(0, &(registers->l));
                
            case 0xC6:
                return setBHl(0, instructionCycle);

            case 0xC7:
                return setBR(0, &(registers->a));

            case 0xC8:
                return setBR(1, &(registers->b));

            case 0xC9:
                return setBR(1, &(registers->c));

            case 0xCA:
                return setBR(1, &(registers->d));
                
            case 0xCB:
                return setBR(1, &(registers->e));
                
            case 0xCC:
                return setBR(1, &(registers->h));
                
            case 0xCD:
                return setBR(1, &(registers->l));
                
            case 0xCE:
                return setBHl(1, instructionCycle);
                
            case 0xCF:
                return setBR(1, &(registers->a));

            case 0xD0:
                return setBR(2, &(registers->b));

            case 0xD1:
                return setBR(2, &(registers->c));

            case 0xD2:
                return setBR(2, &(registers->d));
                
            case 0xD3:
                return setBR(2, &(registers->e));

            case 0xD4:
                return setBR(2, &(registers->h));

            case 0xD5:
                return setBR(2, &(registers->l));

            case 0xD6:
                return setBHl(2, instructionCycle);
                
            case 0xD7:
                return setBR(2, &(registers->a));
                
            case 0xD8:
                return setBR(3, &(registers->b));
                
            case 0xD9:
                return setBR(3, &(registers->c));

            case 0xDA:
                return setBR(3, &(registers->d));

            case 0xDB:
                return setBR(3, &(registers->e));
                
            case 0xDC:
                return setBR(3, &(registers->h));

            case 0xDD:
                return setBR(3, &(registers->l));

            case 0xDE:
                return setBHl(3, instructionCycle);

            case 0xDF:
                return setBR(3, &(registers->a));
                
            case 0xE0:
                return setBR(4, &(registers->b));
                
            case 0xE1:
                return setBR(4, &(registers->c));
                
            case 0xE2:
                return setBR(4, &(registers->d));

            case 0xE3:
                return setBR(4, &(registers->e));

            case 0xE4:
                return setBR(4, &(registers->h));

            case 0xE5:
                return setBR(4, &(registers->l));

            case 0xE6:
                return setBHl(4, instructionCycle);
                
            case 0xE7:
                return setBR(4, &(registers->a));
                
            case 0xE8:
                return setBR(5, &(registers->b));
                
            case 0xE9:
                return setBR(5, &(registers->c));
                
            case 0xEA:
                return setBR(5, &(registers->d));

            case 0xEB:
                return setBR(5, &(registers->e));
                
            case 0xEC:
                return setBR(5, &(registers->h));
                
            case 0xED:
                return setBR(5, &(registers->l));
                
            case 0xEE:
                return setBHl(5, instructionCycle);
                
            case 0xEF:
                return setBR(5, &(registers->a));

            case 0xF0:
                return setBR(6, &(registers->b));

            case 0xF1:
                return setBR(6, &(registers->c));

            case 0xF2:
                return setBR(6, &(registers->d));

            case 0xF3:
                return setBR(6, &(registers->e));

            case 0xF4:
                return setBR(6, &(registers->h));
                
            case 0xF5:
                return setBR(6, &(registers->l));

            case 0xF6:
                return setBHl(6, instructionCycle);

            case 0xF7:
                return setBR(6, &(registers->a));

            case 0xF8:
                return setBR(7, &(registers->b));

            case 0xF9:
                return setBR(7, &(registers->c));

            case 0xFA:
                return setBR(7, &(registers->d));
                
            case 0xFB:
                return setBR(7, &(registers->e));

            case 0xFC:
                return setBR(7, &(registers->h));

            case 0xFD:
                return setBR(7, &(registers->l));
                
            case 0xFE:
                return setBHl(7, instructionCycle);

            case 0xFF:
                return setBR(7, &(registers->a));
        }
    } 
        
    case 0xCC: //CALL Z, nn
        condition = registers->isFlagSet(RegistersFlags::ZERO_FLAG);
        return callCIm(condition, instructionCycle);

    case 0xCD: //CALL nn
        return callIm(instructionCycle);
    
    case 0xCE: //ADC A, n
        return adcImA();
              
    
    case 0xCF: //RST 08h
        return rst(0x0008, instructionCycle);
        

    case 0xD0: //RET NC
        condition = !registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        return retC(condition, instructionCycle);
    
    case 0xD1: //POP DE
        return popR(&(registers->de), instructionCycle);
        
    
    case 0xD2: //JP NC, nn
        condition = !registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        return jpCIm(condition, instructionCycle);
    
    //case 0xD3:
        //empty!
    
    case 0xD4: //CALL NC, nn
        condition = !registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        return callCIm(condition, instructionCycle);
    
    case 0xD5: //PUSH DE
        return pushR(&(registers->de), instructionCycle);
        
    
    case 0xD6: //SUB A, n
        return subImA();
    
    case 0xD7: //RST 10h
        return rst(0x0010, instructionCycle);
    
    case 0xD8: //RET C
        condition = registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        return retC(condition, instructionCycle);

    case 0xD9: //RETI
        return retI(instructionCycle);
    
    case 0xDA: //JP C, nn
        condition = registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        return jpCIm(condition, instructionCycle);
    
    //case 0xDB:
        //empty     
    
    case 0xDC: //CALL C, nn
        condition = registers->isFlagSet(RegistersFlags::CARRY_FLAG);
        return callCIm(condition, instructionCycle);
    
    //case 0x0xDD:
        //empty
    
    case 0xDE: //SBC A, n
        return sbcImA();
    
    case 0xDF: //RST 18h
        return rst(0x0018, instructionCycle);

    case 0xE0: //LD (FF00+n), A
        return loadAHIm(instructionCycle);
    
    case 0xE1: //POP HL
        return popR(&(registers->hl), instructionCycle);
    
    case 0xE2: //LD (FF00+C), A
        return loadAHC();
    
    //0xE3 and 0xE4 are empty
    
    case 0xE5: //PUSH HL
        return pushR(&(registers->hl), instructionCycle);
    
    case 0xE6: //AND A, n
        return andAIm();
        
    
    case 0xE7: //RST 20h
        return rst(0x0020, instructionCycle);
    
    case 0xE8: //ADD SP, n
        return addSpE(instructionCycle);
    
    case 0xE9: //JP HL
        return jpHl();
    
    case 0xEA: //LD (nn), A
        return loadAIm(instructionCycle);
    
    //0xEB, 0xEC and 0xED are empty         
    
    case 0xEE: //XOR A, n
        return xorAIm();
    
    case 0xEF: //RST 28h
        return rst(0x0028, instructionCycle);
    
    case 0xF0: //LD A, (FF00+n)
        return loadHImA(instructionCycle);
        
    case 0xF1: { //POP AF
        bool finished = popR(&(registers->af), instructionCycle);
        if(finished)
            registers->f &= 0xF0;
        return finished;
    }
    
    case 0xF2: //LD A, (FF00+C)
        return loadHCA();
    
    case 0xF3: //DI
        return di();
    
    //0xF4 is empty
    
    case 0xF5: //PUSH AF
        return pushR(&(registers->af), instructionCycle);
    
    case 0xF6: //OR A, n
        return orAIm();
    
    case 0xF7: //RST 30h
        return rst(0x0030, instructionCycle);
    
    case 0xF8: //LD HL, SP+e
        return loadSpEHl(instructionCycle);
              
    
    case 0xF9: //LD SP, HL
        return loadHlSp();
              
    
    case 0xFA: //LD A, nn
        return loadImA(instructionCycle);
    
    case 0xFB: //EI
        return ei();
    
    //0xFC and 0xFD are empty   
    
    case 0xFE: //CP A, n
        return cpAIm();
    
    case 0xFF: //RST 38h
        return rst(0x0038, instructionCycle);
    }
    
    return false;
}

// Basic Instructions
bool Instructions::halt()
{ 
    const uint8_t pending = memory->read(0xFF0F) & memory->read(0xFFFF) & 0x1F;

    if(interrupt->isIMEset())
    {
        interrupt->setHalted(true);
    }
    else if(pending == 0)
    {
        interrupt->setHalted(true);
    }
    else if(pending != 0)
    {
        haltBug = true;
    }

    return true;
}

bool Instructions::stop() {
    //TODO
    return true;
}

bool Instructions::di() {
    interrupt->disableIME();
    return true;
}

bool Instructions::ei() {
    interrupt->scheduleEnableIME();
    return true;
}

// Control flow

bool Instructions::jpIm(int cycle) {

    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }
    else if(cycle == 2)
    {
        readMemoryW = readNext8Bit();
        return false;
    }

    uint16_t address = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;
    registers->pc = address;

    readMemoryW = 0;
    readMemoryZ = 0;
    return true;
}

bool Instructions::jpHl() {
    registers->pc = registers->hl;
    return true;
}

bool Instructions::jrCIm(bool condition, int cycle) {

    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit(); 
        if(!condition)
            return true;
        else
            return false;
    }

    registers->pc += (int8_t)readMemoryZ;

    readMemoryZ = 0;
    return true;

}

bool Instructions::jrIm(int cycle) {
    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }

    registers->pc += (int8_t)readMemoryZ;
    return true;
}

bool Instructions::jpCIm(bool condition, int cycle) {
    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit(); 
        return false;
    }    
    else if(cycle == 2)
    {
        readMemoryW = readNext8Bit(); 
        if(!condition)
            return true;
        else
            return false;
    }

    uint16_t address = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;
    registers->pc = address;

    readMemoryW = 0;
    readMemoryZ = 0;
    return true;
}

bool Instructions::callIm(int cycle) {

    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }    
    else if(cycle == 2)
    {
        readMemoryW = readNext8Bit();
        return false;
    }
    else if(cycle == 3)
    {
        registers->sp--;
        return false;
    }
    else if(cycle == 4)
    {
        memory->write(registers->sp, (registers->pc>>8) & 0xFF);  
        registers->sp--;
        return false;
    }
    
    memory->write(registers->sp, registers->pc & 0xFF);  

    uint16_t jumpAddress = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;
    registers->pc = jumpAddress;  
    
    readMemoryW = 0;
    readMemoryZ = 0;
    return true;
}


bool Instructions::callCIm(bool condition, int cycle) {
    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }    
    else if(cycle == 2)
    {
        readMemoryW = readNext8Bit();
        if(!condition)
            return true;
        else
            return false;
    }
    else if(cycle == 3)
    {
        registers->sp--;
        return false;
    }
    else if(cycle == 4)
    {
        memory->write(registers->sp, (registers->pc>>8) & 0xFF);  
        registers->sp--;
        return false;
    }

    memory->write(registers->sp, registers->pc & 0xFF);  

    uint16_t jumpAddress = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;
    registers->pc = jumpAddress;  
    
    readMemoryW = 0; 
    readMemoryZ = 0;
    return true;
}

bool Instructions::ret(int cycle) {
    if(cycle == 1)
    {
        readMemoryZ = memory->read(registers->sp);
        registers->sp++;
        return false;
    }
    else if(cycle == 2)
    {
        readMemoryW = memory->read(registers->sp);
        registers->sp++;
        return false;
    }

    registers->pc = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;

    readMemoryZ = 0;
    readMemoryW = 0;
    return true;
}

bool Instructions::retC(bool condition, int cycle) {
    if(cycle == 1)
    {
        if(!condition)
            return true;
        else
            return false;
    }
    else if(cycle == 2)
    {
        readMemoryZ = memory->read(registers->sp);
        registers->sp++;
        return false;
    }
    else if(cycle == 3)
    {
        readMemoryW = memory->read(registers->sp);
        registers->sp++;
        return false;
    }

    registers->pc = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;

    readMemoryZ = 0;
    readMemoryW = 0;
    return true;
}

bool Instructions::retI(int cycle) {
    if (cycle == 1)
    {
        readMemoryZ = memory->read(registers->sp);
        registers->sp++;
        return false;
    }
    else if (cycle == 2) 
    {
        readMemoryW = memory->read(registers->sp);
        registers->sp++;
        return false;
    }
    
    interrupt->enableIME();
    registers->pc = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;

    readMemoryW = 0;
    readMemoryZ = 0;
    return true;
}

bool Instructions::rst(uint16_t address, int cycle) {
    if(cycle == 1)
    {
        registers->sp--;
        return false;
    }
    else if (cycle == 2)
    {
        memory->write(registers->sp, (registers->pc >>8) & 0xFF);
        registers->sp--;
        return false;
    }
    
    memory->write(registers->sp, registers->pc & 0xFF);
    registers->pc = address;
    return true;
}

// Load 8 bit
bool Instructions::loadRR8(uint8_t *dest, uint8_t *source) {
    *dest = *source;

    return true;
}

bool Instructions::loadImR8(uint8_t *dest) {
    *dest = readNext8Bit();
    return true;
}

bool Instructions::loadHlR(uint8_t *dest) {
    *dest = memory->read(registers->hl);
    return true;
}

bool Instructions::loadRHl(uint8_t *source) {
    memory->write(registers->hl, *source);
    return true;
}

bool Instructions::loadImHl(int cycle) {
    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }

    memory->write(registers->hl, readMemoryZ);
    return true;
}

bool Instructions::loadRA(uint16_t *reg) {
    memory->write(*reg, registers->a);
    return true;
}

bool Instructions::loadAR(uint16_t *reg) {
    registers->a = memory->read(*reg);

    return true;
}

bool Instructions::loadImA(int cycle) {
    if(cycle == 1)
    { 
        readMemoryZ = readNext8Bit();
        return false;
    }
    else if(cycle == 2)
    {
        readMemoryW = readNext8Bit();
        return false;
    }
    uint16_t address = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;
    registers->a = memory->read(address);

    readMemoryZ = 0;
    readMemoryW = 0; 
    return true;
}

bool Instructions::loadAIm(int cycle) {
    if(cycle == 1)
    { 
        readMemoryZ = readNext8Bit();
        return false;
    }
    else if(cycle == 2)
    {
        readMemoryW = readNext8Bit();
        return false;
    }    
    uint16_t address = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;
    memory->write(address, registers->a);

    readMemoryW = 0;
    readMemoryZ = 0;
    return true;
}

bool Instructions::loadHCA() {
    registers->a = memory->read(0xFF00 + registers->c);
    return true;
}

bool Instructions::loadAHC() {
    memory->write(0xFF00 + registers->c, registers->a);

    return true;
}

bool Instructions::loadAHIm(int cycle) {
    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }

    memory->write(0xFF00 + readMemoryZ, registers->a);

    readMemoryZ = 0;
    return true;
}

bool Instructions::loadHImA(int cycle) {
    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }
    registers->a = memory->read(0xFF00 + readMemoryZ);

    readMemoryZ = 0;
    return true;
}

bool Instructions::loadAHlMinus() {
    memory->write(registers->hl, registers->a);
    registers->hl--;

    return true;
}

bool Instructions::loadHlMinusA() {
    registers->a = memory->read(registers->hl);
    registers->hl--;

    return true;
}

bool Instructions::loadAHlPlus() {
    memory->write(registers->hl, registers->a);
    registers->hl++;

    return true;
}

bool Instructions::loadHlPlusA() {
    registers->a = memory->read(registers->hl);
    registers->hl++;

    return true;
}

// Load 16 bit
bool Instructions::loadImR16(uint16_t *dest, int cycle) {
    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }

    *dest = static_cast<uint16_t>(readNext8Bit() << 8) | readMemoryZ;

    readMemoryZ = 0;
    return true;
}

bool Instructions::loadSpIm(int cycle) {
    
    if(cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }
    else if(cycle == 2)
    {
        readMemoryW = readNext8Bit();
        return false;
    }
    else if(cycle == 3)
    {
        uint16_t immediate = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;
        memory->write(immediate, registers->sp & 0xFF);
        return false;
    }      

    uint16_t immediate = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;
    memory->write(immediate + 1, (registers->sp >> 8) & 0xFF);

    readMemoryW = 0;
    readMemoryZ = 0;
    return true;

}

bool Instructions::loadHlSp() {
    registers->sp = registers->hl;

    return true;
}

bool Instructions::pushR(uint16_t *source, int cycle) {
    if (cycle == 1)
    {
        registers->sp--;
        return false;
    }
    else if (cycle == 2)
    {
        memory->write(registers->sp, (*source >> 8) & 0xFF);
        registers->sp--;
        return false;
    }
    
    memory->write(registers->sp, *source & 0xFF);
    return true;
}

bool Instructions::popR(uint16_t *dest, int cycle) {
    if(cycle == 1)
    {
        readMemoryZ = memory->read(registers->sp);
        registers->sp++;
        return false;
    }

    readMemoryW = memory->read(registers->sp);
    registers->sp++;
    *dest = (static_cast<uint16_t>(readMemoryW) << 8) | readMemoryZ;

    readMemoryZ = 0;
    readMemoryW = 0;
    return true;
}

bool Instructions::loadSpEHl(int cycle) {

    if (cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }
    else if (cycle == 2)
    {
        uint16_t sp = registers->sp;
        int8_t e = (int8_t) readMemoryZ;

        registers->setFlag(RegistersFlags::CARRY_FLAG, ((sp & 0xFF) + (uint8_t)e) > 0xFF);
        registers->setFlag(RegistersFlags::ZERO_FLAG, false);
        registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
        //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, ((sp & 0x0F) + (e & 0x0F)) > 0x0F);
    }
    
    registers->hl = registers->sp + static_cast<int8_t>(readMemoryZ);

    readMemoryZ = 0;
    return true;
  
}

// Arithmetic and logical 8 bit
bool Instructions::addRA(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result = a + *reg;
    registers->a = result;
    
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) + (*reg & 0xF) > 0xF);
    
    return true;
}

bool Instructions::addImA(){
    uint8_t value = readNext8Bit();
    uint8_t a = registers->a;
    uint16_t result = a + value;
    registers->a = result & 0xFF;
    
    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) + (value & 0xF) > 0xF);
    
    return true;
}

bool Instructions::addHlA() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result = a + hlValue;
    registers->a = result;
    
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) + (hlValue & 0xF) > 0xF);

    return true;
}

bool Instructions::adcRA(uint8_t *reg) {
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

    return true;
}

bool Instructions::adcHlA() {
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

    return true;
}

bool Instructions::adcImA() {
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

    return true;
}

bool Instructions::subRA(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result = a - *reg;
    registers->a = result;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, *reg > a);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    //resto per i primi 4 bit. controllo se i primi 4 bit del registro a cui sommo sono maggiori di A
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (*reg & 0xF));

    return true;
}

bool Instructions::subHlA() {
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

    return true;
}

bool Instructions::subImA() {
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

    return true;
}

bool Instructions::sbcRA(uint8_t *reg) {
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
    
    return true;
}

bool Instructions::sbcHlA() {
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
    
    return true;
}

bool Instructions::sbcImA() {
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
    
    return true;
}

bool Instructions::cpAR(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result = a - *reg;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (*reg & 0xF));

    return true;
}

bool Instructions::cpAHl() {
    uint8_t a = registers->a;
    uint8_t hlValue = memory->read(registers->hl);
    uint16_t result = a - hlValue;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (hlValue & 0xF));

    return true;
}

bool Instructions::cpAIm() {
    uint8_t a = registers->a;
    uint8_t value = readNext8Bit();
    uint16_t result = a - value;

    //overflow
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFF);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (result & 0xFF) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (a & 0xF) < (value & 0xF));

    return true;
}

bool Instructions::incR8(uint8_t *reg) {
    uint8_t oldRegValue = *reg;
    (*reg)++;

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (oldRegValue & 0x0F) + 1 > 0x0F);

    return true;
}

bool Instructions::incHl(int cycle) {

    if(cycle == 1)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }

    uint8_t oldZ = readMemoryZ;
    readMemoryZ++;
    memory->write(registers->hl, readMemoryZ);

    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::ZERO_FLAG, readMemoryZ == 0);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (oldZ & 0x0F) + 1 > 0x0F);

    readMemoryZ = 0;
    return true;

}

bool Instructions::decHl(int cycle) {

    if(cycle == 1)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }

    uint8_t oldZ = readMemoryZ;
    readMemoryZ--;
    memory->write(registers->hl, readMemoryZ);

    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    registers->setFlag(RegistersFlags::ZERO_FLAG, readMemoryZ == 0);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (oldZ & 0x0F) == 0);
    
    readMemoryZ = 0;
    return true;
}

bool Instructions::decR8(uint8_t *reg) {
    uint8_t oldRegValue = *reg;
    (*reg)--;

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    
    //half carry qui indica il prestito dal quarto bit. Il prestito avviene solo se i primi 4 bit sono a 0
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (oldRegValue & 0x0F) == 0);

    return true;
}

bool Instructions::andAR(uint8_t *reg) {
    uint8_t a = registers->a & *reg;
    registers->a = a;

    registers->setFlag(RegistersFlags::ZERO_FLAG, a == 0); //true se 0 altrimenti false
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    return true;
}

bool Instructions::andAHl() {
    uint8_t res = registers->a & memory->read(registers->hl);
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0); //true se 0 altrimenti false
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    return true;
}

bool Instructions::andAIm() {
    uint8_t a = registers->a;
    uint8_t res = a & readNext8Bit();
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0); //true se 0 altrimenti false
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    return true;
}

bool Instructions::orAR(uint8_t *reg) {
    uint8_t res = registers->a | *reg;
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    return true;
}

bool Instructions::orAHl() {
    uint8_t res = registers->a | memory->read(registers->hl);
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
    
    return true;
}

bool Instructions::orAIm() {
    uint8_t res = registers->a | readNext8Bit();
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    return true;
}

bool Instructions::xorAR(uint8_t *reg) {
    uint8_t a = registers->a;
    uint8_t res = a ^ *reg;
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    return true;
}

bool Instructions::xorAHl() {
    uint8_t a = registers->a;
    uint8_t res = a ^ memory->read(registers->hl);
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    return true;
}

bool Instructions::xorAIm() {
    uint8_t a = registers->a;
    uint8_t res = a ^ readNext8Bit();
    registers->a = res;

    registers->setFlag(RegistersFlags::ZERO_FLAG, res == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    return true;
}

bool Instructions::ccf() {
    registers->setFlag(RegistersFlags::CARRY_FLAG, !registers->isFlagSet(RegistersFlags::CARRY_FLAG));
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);

    return true;
}

bool Instructions::scf() {
    registers->setFlag(RegistersFlags::CARRY_FLAG, true);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);

    return true;
}


bool Instructions::daA() { 
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

    return true;
}

bool Instructions::cpA() {
    registers->a = ~registers->a;
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, true);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);

    return true;
}

// Arithmetic and logical 16 bit
bool Instructions::incR16(uint16_t *reg, int cycle) {
    (*reg)++;
    return true;
}

bool Instructions::decR16(uint16_t *reg, int cycle) {
    (*reg)--;
    return true;
}

bool Instructions::addHlR(uint16_t *reg) {
    uint16_t hl = registers->hl;
    uint32_t result = hl + *reg;
    registers->hl = result;

    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, (hl & 0x0FFF) + (*reg & 0x0FFF) > 0x0FFF);
    registers->setFlag(RegistersFlags::CARRY_FLAG, result > 0xFFFF);

    return true;
}

bool Instructions::addSpE(int cycle) {

    if (cycle == 1)
    {
        readMemoryZ = readNext8Bit();
        return false;
    }
    else if (cycle == 2)
    {
        uint16_t sp = registers->sp;
        int8_t e = (int8_t)readMemoryZ;
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, 
            ((sp & 0x0F) + (e & 0x0F)) > 0x0F);
        registers->setFlag(RegistersFlags::ZERO_FLAG, false);
        registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
        //overflow from bit 7
        registers->setFlag(RegistersFlags::CARRY_FLAG, 
            ((sp & 0xFF) + (uint8_t)e) > 0xFF);
        return false;
    }
    
    registers->sp += static_cast<int8_t>(readMemoryZ);

    readMemoryZ = 0;
    return true;

}

// Bit operations

bool Instructions::rla() {
    uint8_t a = registers->a;
    uint8_t msb = (a & 0x80) >> 7;
    
    registers->a = a << 1 | registers->isFlagSet(RegistersFlags::CARRY_FLAG);

    registers->setFlag(RegistersFlags::CARRY_FLAG, msb == 1);
    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);

    return true;
}

bool Instructions::rra() {
    uint8_t oldValue = registers->a;
    uint8_t lsb = oldValue & 0x01;
    registers->a = oldValue >> 1 | (registers->isFlagSet(RegistersFlags::CARRY_FLAG) << 7);

    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb == 1);
    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);

    return true;
}

bool Instructions::rlcR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    *reg = oldValue << 1 | bit7;

    registers->setFlag(RegistersFlags::ZERO_FLAG, (*reg) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7==1);

    return true;
}

bool Instructions::rlca() {
    uint8_t oldValue = registers->a;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    registers->a = oldValue << 1 | bit7;

    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7==1);

    return true;
}

bool Instructions::rlcHl(int cycle) {
    if(cycle == 1)
    {
        return false;
    }
    else if(cycle == 2)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }

    uint8_t bit7 = (readMemoryZ & 0x80) >> 7;
    uint8_t result = readMemoryZ << 1 | bit7;
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7==1);

    readMemoryZ = 0;
    return true;
}

bool Instructions::rrcR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t lsb = oldValue & 0x01;
    *reg = oldValue >> 1 | (lsb << 7);

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb==1);

    return true;
}

bool Instructions::rrca() {
    uint8_t oldValue = registers->a;
    uint8_t lsb = oldValue & 0x01;
    registers->a = oldValue >> 1 | (lsb << 7);

    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb==1);

    return true;
}

bool Instructions::rrcHl(int cycle) {
    if(cycle == 1)
    {
        return false;
    }
    else if(cycle == 2)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }

    uint8_t lsb = readMemoryZ & 0x01;
    uint8_t result = readMemoryZ >> 1 | (lsb << 7);
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb==1);

    readMemoryZ = 0;
    return true;
}

bool Instructions::rlR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    *reg = oldValue << 1 | carry;

    registers->setFlag(RegistersFlags::ZERO_FLAG, (*reg) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7);

    return true;
}

bool Instructions::rlHl(int cycle) {
    if(cycle == 1)
    {
        return false;
    }
    else if (cycle == 2)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }
    
    uint8_t bit7 = (readMemoryZ & 0x80) >> 7;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    uint8_t result = readMemoryZ<< 1 | carry;
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7);

    readMemoryZ = 0;
    return true;
}

bool Instructions::rrR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t lsb = oldValue & 0x01;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    *reg = oldValue >> 1 | (carry << 7);

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);

    return true;
}

bool Instructions::rrHl(int cycle) {
    if(cycle == 1)
    {
        return false;
    }
    else if (cycle == 2)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }
    
    uint8_t lsb = readMemoryZ & 0x01;
    uint8_t carry = registers->isFlagSet(RegistersFlags::CARRY_FLAG) ? 1 : 0;
    uint8_t result = readMemoryZ >> 1 | (carry << 7);
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);

    readMemoryZ = 0;
    return true;
}

bool Instructions::slaR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t bit7 = (oldValue & 0x80) >> 7;
    *reg = oldValue << 1;

    registers->setFlag(RegistersFlags::ZERO_FLAG, (*reg) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7);

    return true;
}

bool Instructions::slaHl(int cycle) {
    if (cycle == 1)
    {
        return false;
    }
    else if (cycle == 2)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }
    
    uint8_t bit7 = (readMemoryZ & 0x80) >> 7;
    uint8_t result = readMemoryZ << 1;
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7);

    return true;
}

bool Instructions::sraR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t lsb = oldValue & 0x01;
    uint8_t bit7 = (oldValue & 0x80);
    *reg = (oldValue >> 1) | bit7;

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);

    return true;
}

bool Instructions::sraHl(int cycle) {
    if (cycle == 1)
    {
        return false;
    }
    else if (cycle == 2)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }

    uint8_t lsb = readMemoryZ & 0x01;
    uint8_t bit7 = readMemoryZ & 0x80;
    uint8_t result = (readMemoryZ >> 1) | bit7;
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);

    readMemoryZ = 0;
    return true;
}

bool Instructions::swapR(uint8_t *reg) {
     uint8_t oldValue = *reg;
    *reg = (((oldValue & 0xF0) >> 4) | ((oldValue & 0x0F) << 4));

    registers->setFlag(RegistersFlags::ZERO_FLAG, *reg == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    return true;
}

bool Instructions::swapHl(int cycle) {
    if (cycle == 1)
    {
        return false;
    }
    else if (cycle == 2)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }

    uint8_t result = (((readMemoryZ & 0xF0) >> 4) | ((readMemoryZ & 0x0F) << 4));
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    readMemoryZ = 0;
    return true;
}

bool Instructions::srlR(uint8_t *reg) {
    uint8_t oldValue = *reg;
    uint8_t lsb = oldValue & 0x01;
    *reg = oldValue >> 1;

    registers->setFlag(RegistersFlags::ZERO_FLAG, (*reg) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);

    return true;

    return true;
}

bool Instructions::srlHl(int cycle) {
    if (cycle == 1)
    {
        return false;
    }
    else if (cycle == 2)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }

    uint8_t lsb = readMemoryZ & 0x01;
    uint8_t result = readMemoryZ >> 1;
    memory->write(registers->hl, result);

    registers->setFlag(RegistersFlags::ZERO_FLAG, result == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, lsb);

    readMemoryZ = 0;
    return true;
}

bool Instructions::bitBR(uint8_t bit, uint8_t *reg) {
    registers->setFlag(RegistersFlags::ZERO_FLAG, ((*reg) & (1 << bit)) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);

    return true;
}

bool Instructions::bitBHl(uint8_t bit, int cycle) {
    if(cycle == 1)
    {
        return false;
    }

    readMemoryZ = memory->read(registers->hl);
    registers->setFlag(RegistersFlags::ZERO_FLAG, (readMemoryZ & (1 << bit)) == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);

    readMemoryZ = 0;
    return true;
}

bool Instructions::resBR(uint8_t bit, uint8_t *reg) {
    *reg &= ~(1 << bit);
    return true;
}

bool Instructions::resBHl(uint8_t bit, int cycle) {
    if(cycle == 1)
    {
        return false;
    }
    else if (cycle == 2)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }
    
    memory->write(registers->hl, (readMemoryZ & ~(1 << bit)));

    readMemoryZ = 0;
    return true;
}

bool Instructions::setBR(uint8_t bit, uint8_t *reg) {
    *reg |= (1 << bit);

    return true;
}

bool Instructions::setBHl(uint8_t bit, int cycle) {
    if (cycle == 1)
    {
        return false;
    }
    else if (cycle == 2)
    {
        readMemoryZ = memory->read(registers->hl);
        return false;
    }
    
    memory->write(registers->hl, (readMemoryZ | (1 << bit)));

    readMemoryZ = 0;
    return true;
}

//Grazie chatGPT non avevo voglia
bool Instructions::isSingleMachineCycle(uint8_t opcode)
{
    static constexpr uint8_t machineCycles[256] = {
        1,3,2,2,1,1,2,1,5,2,2,2,1,1,2,1,
        0,3,2,2,1,1,2,1,3,2,2,2,1,1,2,1,
        2,3,2,2,1,1,2,1,2,2,2,2,1,1,2,1,
        2,3,2,2,3,3,3,1,2,2,2,2,1,1,2,1,
        1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
        1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
        1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
        2,2,2,2,2,2,0,2,1,1,1,1,1,1,2,1,
        1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
        1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
        1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
        1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
        2,3,3,4,3,4,2,4,2,4,3,0,3,6,2,4,
        2,3,3,0,3,4,2,4,2,4,3,0,3,0,2,4,
        3,3,2,0,0,4,2,4,4,1,4,0,0,0,2,4,
        3,3,2,1,0,4,2,4,3,2,4,1,0,0,2,4
    };

    // HALT viene gestito subito dopo il fetch.
    if (opcode == 0x76) {
        return false;
    }

    return machineCycles[opcode] != 1;
}
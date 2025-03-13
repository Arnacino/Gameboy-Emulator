#include "Instructions.h"
#include <iostream>

Instructions::Instructions(Registers *registers, Memory *memory) {
    this->registers = registers;
    this->memory = memory;
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
    
    case 0x00: //NOP
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
        loadHlAPlus();
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
        loadHlAMinus();
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

    }
}

// Basic Instructions
void Instructions::halt() {
    //non so, dipende da una IME FLAG. non so cosa sia qwyufgwqyugfqwy
}

void Instructions::stop() {
    //non lo so da capire meglio 
    //https://gbdev.io/pandocs/Reducing_Power_Consumption.html#using-the-stop-instruction

}

void Instructions::di() {}
void Instructions::ei() {}

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

void Instructions::jpCIm() {}
void Instructions::callIm() {}
void Instructions::callCIm() {}
void Instructions::ret() {}
void Instructions::retC() {}
void Instructions::retI() {}
void Instructions::rstIm() {}

// Load 8 bit
void Instructions::loadRR8(uint8_t *dest, uint8_t *source) {
    
    *dest = *source;
    
}

//DA IMPLEMENTARE READNEXT8BIT
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
    registers->a = *reg;
}

void Instructions::loadAR(uint16_t *reg) {
    *reg = registers->a;
}

void Instructions::loadAdA() {}
void Instructions::loadAAd() {}
void Instructions::loadHCA() {}
void Instructions::loadHAC() {}
void Instructions::loadHAIm() {}
void Instructions::loadHImA() {}

void Instructions::loadAHlMinus() {
    uint8_t a = registers->a;
    uint16_t hl = registers->hl;
    memory->write(hl, a);
    registers->hl--;
}
    
void Instructions::loadHlAMinus() {
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

void Instructions::loadHlAPlus() {
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

void Instructions::loadSpHl() {}
void Instructions::pushR(uint16_t *source) {}
void Instructions::popR(uint16_t *source) {}
void Instructions::loadSpEHl() {}

// Arithmetic and logical 8 bit
void Instructions::addAR(uint8_t *reg) {
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

void Instructions::addAHl() {}

void Instructions::adcAR(uint8_t *reg) {
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

void Instructions::adcAHl() {}
void Instructions::adcAIm() {}

void Instructions::subAR(uint8_t *reg) {

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

void Instructions::subAHl() {}
void Instructions::subAIm() {}

void Instructions::sbcAR(uint8_t *reg) {
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
    if((a & 0xF) + (*reg & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
    
    registers->a = result && 0xFF;
}

void Instructions::sbcAHl() {}
void Instructions::sbcAIm() {}

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
    if((a & 0xF) + (*reg & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, true);
    }
}

void Instructions::cpAHl() {}
void Instructions::cpAIm() {}

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
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);
}

void Instructions::andAHl() {}
void Instructions::andAIm() {}

void Instructions::orAR(uint8_t *reg) {
    uint8_t a = registers->a | *reg;
    registers->a = a;

    registers->setFlag(RegistersFlags::ZERO_FLAG, a & 0xFF == 0);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
    registers->setFlag(RegistersFlags::CARRY_FLAG, false);

    
}

void Instructions::orAHl() {}
void Instructions::orAIm() {}
void Instructions::xorAR(uint8_t *reg) {}
void Instructions::xorAHl() {}
void Instructions::xorAIm() {}

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
void Instructions::incR16(uint16_t *reg) {}

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

void Instructions::addSpE() {}

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
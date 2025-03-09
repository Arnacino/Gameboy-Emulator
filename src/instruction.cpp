#include "Instructions.h"

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

    //FINO A QUA

    case 0x18: // INC DE
        incR16(&(registers->de));
        break;

    case 0x19: // INC DE
        incR16(&(registers->de));
        break;

    case 0x1A: // INC DE
        incR16(&(registers->de));
        break; 

    case 0x1B: // INC DE
        incR16(&(registers->de));
        break;

    case 0x1C: // INC DE
        incR16(&(registers->de));
        break;

    case 0x1D: // INC DE
        incR16(&(registers->de));
        break;

    case 0x1E: // INC DE
        incR16(&(registers->de));
        break;

    case 0x1F: // INC DE
        incR16(&(registers->de));
        break;

    case 0x20: // INC DE
        incR16(&(registers->de));
        break;

    case 0x21: // LD HL, nn
        loadImR16(&(registers->hl));
        break;
    
    case 0x23: // INC HL
        incR16(&(registers->hl));
        break;
        
    case 0x31: // LD SP, nn
        loadImR16(&(registers->sp));
        break;

    case 0x33: // INC SP
        incR16(&(registers->sp));
        break;

    }
}
// Basic Instructions
void Instructions::halt() {}
void Instructions::stop() {
    //non lo so da capire meglio 
    //https://gbdev.io/pandocs/Reducing_Power_Consumption.html#using-the-stop-instruction

}
void Instructions::di() {}
void Instructions::ei() {}

// Control flow
void Instructions::jpIm() {}
void Instructions::jpHl() {}
void Instructions::jpCIm() {}
void Instructions::jrE() {}
void Instructions::jrCE() {}
void Instructions::callIm() {}
void Instructions::callCIm() {}
void Instructions::ret() {}
void Instructions::retC() {}
void Instructions::retI() {}
void Instructions::rstIm() {}

// Load 8 bit
void Instructions::loadRR8(uint8_t *source, uint8_t *dest) {
    *dest = *source;
}

void Instructions::loadImR8(uint8_t *dest) {
    uint8_t immediate = readNext8Bit();
    *dest = immediate;
}

void Instructions::loadHlR(uint8_t *dest) {}
void Instructions::loadRHl(uint8_t *source) {}
void Instructions::loadImHl() {}

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
void Instructions::loadAHlMinus() {}
void Instructions::loadHlAMinus() {}
void Instructions::loadAHlPlus() {}
void Instructions::loadHlAPlus() {}

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
    registers->a = result && 0xFF;

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
    registers->a = result && 0xFF;

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
}

void Instructions::adcAHl() {}
void Instructions::adcAIm() {}

void Instructions::subAR(uint8_t *reg) {
    uint8_t a = registers->a;
    uint16_t result = a - *reg;
    registers->a = result && 0xFF;

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

void Instructions::incHl() {}

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

void Instructions::decHl() {}

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
void Instructions::ccf() {}
void Instructions::scf() {}
void Instructions::daA() {}
void Instructions::cpA() {}

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
    registers->setFlag(RegistersFlags::CARRY_FLAG, bit7==1);
    registers->setFlag(RegistersFlags::ZERO_FLAG, false);
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG, false);
    registers->setFlag(RegistersFlags::HALF_CARRY_FLAG, false);
}
void Instructions::rra() {}

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
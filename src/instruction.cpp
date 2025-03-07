#include "Instructions.h"

Instructions::Instructions(Registers *registers) {
    this->registers = registers;
}

// Basic Instructions
void Instructions::halt() {}
void Instructions::stop() {}
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
void Instructions::loadImR8(uint8_t *dest) { }
void Instructions::loadHlR(uint8_t *dest) {}
void Instructions::loadRHl(uint8_t *source) {}
void Instructions::loadImHl() {}
void Instructions::loadBCA() {}
void Instructions::loadDEA() {}
void Instructions::loadABC() {}
void Instructions::loadADE() {}
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
void Instructions::loadImR16(uint16_t *dest) {}
void Instructions::loadSpA() {}
void Instructions::loadSpHl() {}
void Instructions::pushR(uint16_t *source) {}
void Instructions::popR(uint16_t *source) {}
void Instructions::loadSpEHl() {}

// Arithmetic and logical 8 bit
void Instructions::addAR(uint8_t *operand) {
    uint8_t a = registers->a;
    uint16_t result = a + *operand;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG);
    }
    if(result & 0xFF == 0){
        registers->setFlag(RegistersFlags::ZERO_FLAG);
    }
    registers->setFlag(~RegistersFlags::SUBTRACTION_FLAG);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (*operand & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG);
    }
    
    registers->a = result && 0xFF;
}

void Instructions::addAHl() {}

void Instructions::adcAR(uint8_t *operand) {
    uint8_t a = registers->a;
    uint16_t result;
    if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
        result = a + *operand + 1;
    }else{
        result = a + *operand;
    }

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG);
    }
    if(result & 0xFF == 0){
        registers->setFlag(RegistersFlags::ZERO_FLAG);
    }
    registers->setFlag(~RegistersFlags::SUBTRACTION_FLAG);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (*operand & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG);
    }
    
    registers->a = result && 0xFF;
}

void Instructions::adcAHl() {}
void Instructions::adcAIm() {}

void Instructions::subAR(uint8_t *operand) {
    uint8_t a = registers->a;
    uint16_t result = a - *operand;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG);
    }
    if(result & 0xFF == 0){
        registers->setFlag(RegistersFlags::ZERO_FLAG);
    }
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (*operand & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG);
    }
    
    registers->a = result && 0xFF;
}

void Instructions::subAHl() {}
void Instructions::subAIm() {}

void Instructions::sbcAR(uint8_t *operand) {
    uint8_t a = registers->a;
    uint16_t result;
    if(registers->isFlagSet(RegistersFlags::CARRY_FLAG)){
        result = a - *operand - 1;
    }else {
        result = a - *operand;
    }

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG);
    }
    if(result & 0xFF == 0){
        registers->setFlag(RegistersFlags::ZERO_FLAG);
    }
    registers->setFlag(~RegistersFlags::SUBTRACTION_FLAG);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (*operand & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG);
    }
    
    registers->a = result && 0xFF;
}

void Instructions::sbcAHl() {}
void Instructions::sbcAIm() {}

void Instructions::cpAR(uint8_t *operand) {
    uint8_t a = registers->a;
    uint16_t result = a - *operand;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG);
    }
    if(result & 0xFF == 0){
        registers->setFlag(RegistersFlags::ZERO_FLAG);
    }
    registers->setFlag(RegistersFlags::SUBTRACTION_FLAG);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((a & 0xF) + (*operand & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG);
    }
}

void Instructions::cpAHl() {}
void Instructions::cpAIm() {}

void Instructions::incR8(uint8_t *operand) {
    uint16_t result = *operand + 1;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG);
    }
    if(result & 0xFF == 0){
        registers->setFlag(RegistersFlags::ZERO_FLAG);
    }
    registers->setFlag(~RegistersFlags::SUBTRACTION_FLAG);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((1 & 0xF) + (*operand & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG);
    }
}

void Instructions::incHl() {}

void Instructions::decR8(uint8_t *operand) {
    uint16_t result = *operand - 1;

    //overflow
    if(result > 0xFF){
        registers->setFlag(RegistersFlags::CARRY_FLAG);
    }
    if(result & 0xFF == 0){
        registers->setFlag(RegistersFlags::ZERO_FLAG);
    }
    registers->setFlag(~RegistersFlags::SUBTRACTION_FLAG);
    
    //resto per i primi 4 bit. Sommo a + registro nei primi 4 bit e se è > 1111 allora metto flag
    if((1 & 0xF) + (*operand & 0xF) > 0xF){
        registers->setFlag(RegistersFlags::HALF_CARRY_FLAG);
    }
}

void Instructions::decHl() {}

void Instructions::andAR(uint8_t *operand) {
    uint8_t a = registers->a & *operand;

    if(a & 0xFF == 0){
        registers->setFlag(RegistersFlags::ZERO_FLAG);
    }
    registers->setFlag(~RegistersFlags::SUBTRACTION_FLAG);
    registers->setFlag(~RegistersFlags::HALF_CARRY_FLAG);
    registers->setFlag(~RegistersFlags::CARRY_FLAG);

    registers->a = a;

}

void Instructions::andAHl() {}
void Instructions::andAIm() {}

void Instructions::orAR(uint8_t *operand) {
    uint8_t a = registers->a | *operand;

    if(a & 0xFF == 0){
        registers->setFlag(RegistersFlags::ZERO_FLAG);
    }
    registers->setFlag(~RegistersFlags::SUBTRACTION_FLAG);
    registers->setFlag(~RegistersFlags::HALF_CARRY_FLAG);
    registers->setFlag(~RegistersFlags::CARRY_FLAG);

    registers->a = a;
}

void Instructions::orAHl() {}
void Instructions::orAIm() {}
void Instructions::xorAR(uint8_t *operand) {}
void Instructions::xorAHl() {}
void Instructions::xorAIm() {}
void Instructions::ccf() {}
void Instructions::scf() {}
void Instructions::daA() {}
void Instructions::cpA() {}

// Arithmetic and logical 16 bit
void Instructions::incR16(uint16_t *operand) {}
void Instructions::decR16(uint16_t *operand) {}
void Instructions::addHlR(uint16_t *operand) {}
void Instructions::addSpE() {}

// Bit operations
void Instructions::rlca() {}
void Instructions::rrca() {}
void Instructions::rla() {}
void Instructions::rra() {}
void Instructions::rlcR(uint8_t *operand) {}
void Instructions::rlcHl() {}
void Instructions::rrcR(uint8_t *operand) {}
void Instructions::rrcHl() {}
void Instructions::rrR(uint8_t *operand) {}
void Instructions::rrHl() {}
void Instructions::slaR(uint8_t *operand) {}
void Instructions::slaHl() {}
void Instructions::sraR(uint8_t *operand) {}
void Instructions::sraHl() {}
void Instructions::swapR(uint8_t *operand) {}
void Instructions::swapHl() {}
void Instructions::srlR(uint8_t *operand) {}
void Instructions::srlHl() {}
void Instructions::bitBR(uint8_t *operand) {}
void Instructions::bitBHl() {}
void Instructions::resBR(uint8_t *operand) {}
void Instructions::resBHl() {}
void Instructions::setBR(uint8_t *operand) {}
void Instructions::setBHl() {}
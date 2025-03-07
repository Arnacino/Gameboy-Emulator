#include "instructions.h"

// Basic instructions
void instructions::halt() {}
void instructions::stop() {}
void instructions::di() {}
void instructions::ei() {}

// Control flow
void instructions::jpIm() {}
void instructions::jpHl() {}
void instructions::jpCIm() {}
void instructions::jrE() {}
void instructions::jrCE() {}
void instructions::callIm() {}
void instructions::callCIm() {}
void instructions::ret() {}
void instructions::retC() {}
void instructions::retI() {}
void instructions::rstIm() {}

// Load 8 bit
void instructions::loadRR8(uint8_t *source, uint8_t *dest) {
    *dest = *source;
}
void instructions::loadImR8(uint8_t *dest) { 
}
void instructions::loadHlR(uint8_t *dest) {}
void instructions::loadRHl(uint8_t *source) {}
void instructions::loadImHl() {}
void instructions::loadBCA() {}
void instructions::loadDEA() {}
void instructions::loadABC() {}
void instructions::loadADE() {}
void instructions::loadAdA() {}
void instructions::loadAAd() {}
void instructions::loadHCA() {}
void instructions::loadHAC() {}
void instructions::loadHAIm() {}
void instructions::loadHImA() {}
void instructions::loadAHlMinus() {}
void instructions::loadHlAMinus() {}
void instructions::loadAHlPlus() {}
void instructions::loadHlAPlus() {}

// Load 16 bit
void instructions::loadImR16(uint16_t *dest) {}
void instructions::loadSpA() {}
void instructions::loadSpHl() {}
void instructions::pushR(uint16_t *source) {}
void instructions::popR(uint16_t *source) {}
void instructions::loadSpEHl() {}

// Arithmetic and logical 8 bit
void instructions::addAR(uint8_t *operand) {}
void instructions::addAHl() {}
void instructions::adcAR(uint8_t *operand) {}
void instructions::adcAHl() {}
void instructions::adcAIm() {}
void instructions::subAR(uint8_t *operand) {}
void instructions::subAHl() {}
void instructions::subAIm() {}
void instructions::sbcAR(uint8_t *operand) {}
void instructions::sbcAHl() {}
void instructions::sbcAIm() {}
void instructions::cpAR(uint8_t *operand) {}
void instructions::cpAHl() {}
void instructions::cpAIm() {}
void instructions::incR8(uint8_t *operand) {}
void instructions::incHl() {}
void instructions::decR8(uint8_t *operand) {}
void instructions::decHl() {}
void instructions::andAR(uint8_t *operand) {}
void instructions::andAHl() {}
void instructions::andAIm() {}
void instructions::orAR(uint8_t *operand) {}
void instructions::orAHl() {}
void instructions::orAIm() {}
void instructions::xorAR(uint8_t *operand) {}
void instructions::xorAHl() {}
void instructions::xorAIm() {}
void instructions::ccf() {}
void instructions::scf() {}
void instructions::daA() {}
void instructions::cpA() {}

// Arithmetic and logical 16 bit
void instructions::incR16(uint16_t *operand) {}
void instructions::decR16(uint16_t *operand) {}
void instructions::addHlR(uint16_t *operand) {}
void instructions::addSpE() {}

// Bit operations
void instructions::rlca() {}
void instructions::rrca() {}
void instructions::rla() {}
void instructions::rra() {}
void instructions::rlcR(uint8_t *operand) {}
void instructions::rlcHl() {}
void instructions::rrcR(uint8_t *operand) {}
void instructions::rrcHl() {}
void instructions::rrR(uint8_t *operand) {}
void instructions::rrHl() {}
void instructions::slaR(uint8_t *operand) {}
void instructions::slaHl() {}
void instructions::sraR(uint8_t *operand) {}
void instructions::sraHl() {}
void instructions::swapR(uint8_t *operand) {}
void instructions::swapHl() {}
void instructions::srlR(uint8_t *operand) {}
void instructions::srlHl() {}
void instructions::bitBR(uint8_t *operand) {}
void instructions::bitBHl() {}
void instructions::resBR(uint8_t *operand) {}
void instructions::resBHl() {}
void instructions::setBR(uint8_t *operand) {}
void instructions::setBHl() {}
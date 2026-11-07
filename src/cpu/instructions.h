#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <cstdint>

class Registers;
class Interrupt;
class Instructions;
class Memory;

class Instructions
{

private:
    Registers *registers;
    Memory *memory;
    Interrupt *interrupt;

    uint8_t readMemoryZ = 0;
    uint8_t readMemoryW = 0;

    uint8_t cbOpcode = 0;

    bool halt();
    bool stop();
    bool di(); //disable interrupts
    bool ei(); //enable interrupts

    //control flow
    bool jpIm(int cycle); //jumps to immediate (16 bit immediate)
    bool jpHl(); //jumps to HL
    bool jpCIm(bool condition, int cycle); //jumps depending on the condition
    bool jrIm(int cycle); //jumps relative to E
    bool jrCIm(bool condition, int cycle); //jumps depending on the condition, jumps relative to e
    bool callIm(int cycle); //calls the function at adress immediate
    bool callCIm(bool condition, int cycle); //same as above but conditional
    bool ret(int cycle); //return from function
    bool retC(bool condition, int cycle); //conditional return
    bool retI(int cycle); //return from interrupt
    bool rst(uint16_t address, int cycle); //restart

    //load 8 bit (a destra c'è sempre dove finisce il risultato)
    bool loadRR8(uint8_t *source, uint8_t *dest); //load data from r1 to r2
    bool loadImR8(uint8_t *dest); //load immediate to register (8 bit)
    bool loadHlR(uint8_t *dest); //load data from the adress specified by HL to register 
    bool loadRHl(uint8_t *source); //load data from register to the adress specified by Hl
    bool loadImHl(int cycle); //load immediate to the adress specified by Hl
    bool loadRA(uint16_t *reg); //load data from register to A to register
    bool loadAR(uint16_t *reg); //load data from A to the adress specified by register
    bool loadImA(int cycle); //load data from the adress specified to A
    bool loadAIm(int cycle); //load data from A to the absolute adress specified
    bool loadHCA(); //load data from 0xFF00 + C to A
    bool loadAHC(); //load data from A to 0xFF00 + C
    bool loadHImA(int cycle); //load data from 0xFF00 + Immediate (8 bit) to A
    bool loadAHIm(int cycle); //load data from A to the adress 0xFF00 + Immediate 
    bool loadHlMinusA(); //load data from the adress specified by HL to A. Decrements HL
    bool loadAHlMinus(); //load data from A to the absolute adress specified by HL. Decrements HL 
    bool loadHlPlusA(); //load data from the adress specified by HL to A. Increments HL
    bool loadAHlPlus(); //load data from A to the absolute adress specified by HL. Increments HL 

    //load 16 bit
    bool loadImR16(uint16_t *dest, int cycle); //load immediate to register (16 bit)
    bool loadSpIm(int cycle); //load stack pointer adress to adress specified (16 bit)
    bool loadHlSp(); //load hl to the stack pointer
    bool pushR(uint16_t *source, int cycle); //push to the stack memory data from R (16 bit register)
    bool popR(uint16_t *source, int cycle); //same but pop
    bool loadSpEHl(int cycle); //load Stack pointer + E (signed) to hl

    //aritmetiche e logiche 8 bit
    bool addRA(uint8_t *reg); //does A+R and stores it in A where R is a register passed
    bool addHlA(); //does A+valueOf(Hl) and stores it in A
    bool addImA(); //does A + immediate and stores it in A (8 bit)
    bool adcRA(uint8_t *reg); //does A+R and stores it in A. adds an extra 1 if carry flag is set
    bool adcHlA(); //does A+valueOf(Hl) and stores it in A. adds an extra 1 if carry flag is set
    bool adcImA(); //does A+Immediate and stores it in A. adds an extra 1 if carry flag is set
    bool subRA(uint8_t *reg); //does A-R and stores it in A
    bool subHlA(); //does A-valueOf(HL) and stores it in A
    bool subImA(); //does A-Immediate and stores it in A
    bool sbcRA(uint8_t *reg); //does A-R with carry (removes an extra 1 if carry flag)
    bool sbcHlA(); //does A-valueOf(HL) with carry
    bool sbcImA(); //does A-Immediate with carry
    bool cpAR(uint8_t *reg); //does A-R and changes flags based on result
    bool cpAHl(); //does A-valueOf(HL) and changes flags based on result
    bool cpAIm(); //does A-Immediate and changes flags based on result
    bool incR8(uint8_t *reg); //reads R and does valueOf(R) + 1
    bool incHl(int cycle); //reads the absolute adress in HL and does + 1
    bool decR8(uint8_t *reg); //reads R and does valueOf(R) - 1
    bool decHl(int cycle); //reads the absolute adress in HL and does - 1
    bool andAR(uint8_t *reg); //does A and R and stores in A
    bool andAHl(); //does A and valueIn(Hl) and stores in A
    bool andAIm(); //does A and Immediate and stores in A
    bool orAR(uint8_t *reg); //does A or R and stores in A
    bool orAHl(); //does A or valueIn(Hl) and stores in A
    bool orAIm(); //does A or Immediate and stores in A
    bool xorAR(uint8_t *reg); //does A xor R and stores in A
    bool xorAHl(); //does A xor valueIn(Hl) and stores in A
    bool xorAIm(); //does A xor Immediate and stores in A
    bool ccf(); //flips the value of the carry flag. Also clears N flag and H flag.
    bool scf(); //sets the carry flag and clears the N flag and H flag
    bool daA(); //complicata leggere OBSIDIAN
    bool cpA(); //flips all bits in A, sets N and H flags

    //aritmetiche e logiche 16 bit
    bool incR16(uint16_t *reg, int cycle); //reads R and does valueof(R) + 1
    bool decR16(uint16_t *reg, int cycle); //reads R and does valueof(R) - 1
    bool addHlR(uint16_t *reg); //does HL + R and stores in HL
    bool addSpE(int cycle); //does Sp + E and stores in Sp. (E is 8 bit)
    
    bool rrca();
    bool rla();
    bool rra();
    bool rlca();
    bool rlcR(uint8_t *reg);
    bool rlcHl(int cycle);
    bool rrcR(uint8_t *reg);
    bool rrcHl(int cycle);
    bool rrR(uint8_t *reg); 
    bool rlR(uint8_t *reg); 
    bool rrHl(int cycle);
    bool rlHl(int cycle);
    bool slaR(uint8_t *reg); 
    bool slaHl(int cycle);
    bool sraR(uint8_t *reg); 
    bool sraHl(int cycle);
    bool swapR(uint8_t *reg); 
    bool swapHl(int cycle);
    bool srlR(uint8_t *reg); 
    bool srlHl(int cycle);
    bool bitBR(uint8_t bit, uint8_t *reg);
    bool bitBHl(uint8_t bit, int cycle);
    bool resBR(uint8_t bit, uint8_t *reg); 
    bool resBHl(uint8_t bit, int cycle);
    bool setBR(uint8_t bit, uint8_t *reg); 
    bool setBHl(uint8_t bit, int cycle);
    uint8_t readNext8Bit();
    
public:
    Instructions(Registers *registers, Memory *memory, Interrupt *interrupt);


    bool execute(uint8_t opcode, int instructionCycle);
    bool isSingleMachineCycle(uint8_t opcode);
};

#endif
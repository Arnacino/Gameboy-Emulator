#include <stdint.h>

class instructions
{
public:
    void halt();
    void stop();
    void di(); //disable interrupts
    void ei(); //enable interrupts

    //control flow
    void jpIm(); //jumps to immediate (16 bit immediate)
    void jpHl(); //jumps to HL
    void jpCIm(); //jumps depending on the condition
    void jrE(); //jumps relative to E
    void jrCE(); //jumps depending on the condition, jumps relative to e
    void callIm(); //calls the function at adress immediate
    void callCIm(); //same as above but conditional
    void ret(); //return from function
    void retC(); //conditional return
    void retI(); //return from interrupt
    void rstIm(); //restart

    //load 8 bit
    void loadRR8(uint8_t *source, uint8_t *dest); //load data from r1 to r2
    void loadImR8(uint8_t *dest); //load immediate to register (8 bit)
    void loadHlR(uint8_t *dest); //load data from the adress specified by HL to register 
    void loadRHl(uint8_t *source); //load data from register to the adress specified by Hl
    void loadImHl(); //load immediate to the adress specified by Hl
    void loadBCA(); //load data from BC to A 
    void loadDEA(); //load data from DE to A
    void loadABC(); //load data from A to the adress specified by BC
    void loadADE(); //load data from A to the adress specified by DE
    void loadAdA(); //load data from the adress specified to A
    void loadAAd(); //load data from A to the absolute adress specified
    void loadHCA(); //load data from 0xFF00 + C to register
    void loadHAC(); //load data from A to 0xFF00 + C
    void loadHAIm(); //load data from 0xFF00 + Immediate (8 bit) to A
    void loadHImA(); //load data from A to the adress 0xFF00 + Immediate 
    void loadAHlMinus(); //load data from the adress specified by HL to A. Decrements HL
    void loadHlAMinus(); //load data from A to the absolute adress specified by HL. Decrements HL 
    void loadAHlPlus(); //load data from the adress specified by HL to A. Increments HL
    void loadHlAPlus(); //load data from A to the absolute adress specified by HL. Increments HL 

    //load 16 bit
    void loadImR16(uint16_t *dest); //load immediate to register (16 bit)
    void loadSpA(); //load stack pointer adress to adress specified (16 bit)
    void loadSpHl(); //load hl to the stack pointer
    void pushR(uint16_t *source); //push to the stack memory data from R (16 bit register)
    void popR(uint16_t *source); //same but pop
    void loadSpEHl(); //load Stack pointer + E (signed) to hl

    //aritmetiche e logiche 8 bit
    void addAR(uint8_t *operand); //does A+R and stores it in A where R is a register passed
    void addAHl(); //does A+valueOf(Hl) and stores it in A
    void adcAR(uint8_t *operand); //does A+R and stores it in A. adds an extra 1 if carry flag is set
    void adcAHl(); //does A+valueOf(Hl) and stores it in A. adds an extra 1 if carry flag is set
    void adcAIm(); //does A+Immediate and stores it in A. adds an extra 1 if carry flag is set
    void subAR(uint8_t *operand); //does A-R and stores it in A
    void subAHl(); //does A-valueOf(HL) and stores it in A
    void subAIm(); //does A-Immediate and stores it in A
    void sbcAR(uint8_t *operand); //does A-R with carry (removes an extra 1 if carry flag)
    void sbcAHl(); //does A-valueOf(HL) with carry
    void sbcAIm(); //does A-Immediate with carry
    void cpAR(uint8_t *operand); //does A-R and changes flags based on result
    void cpAHl(); //does A-valueOf(HL) and changes flags based on result
    void cpAIm(); //does A-Immediate and changes flags based on result
    void incR8(uint8_t *operand); //reads R and does valueOf(R) + 1
    void incHl(); //reads the absolute adress in HL and does + 1
    void decR8(uint8_t *operand); //reads R and does valueOf(R) - 1
    void decHl(); //reads the absolute adress in HL and does - 1
    void andAR(uint8_t *operand); //does A and R and stores in A
    void andAHl(); //does A and valueIn(Hl) and stores in A
    void andAIm(); //does A and Immediate and stores in A
    void orAR(uint8_t *operand); //does A or R and stores in A
    void orAHl(); //does A or valueIn(Hl) and stores in A
    void orAIm(); //does A or Immediate and stores in A
    void xorAR(uint8_t *operand); //does A xor R and stores in A
    void xorAHl(); //does A xor valueIn(Hl) and stores in A
    void xorAIm(); //does A xor Immediate and stores in A
    void ccf(); //flips the value of the carry flag. Also clears N flag and H flag.
    void scf(); //sets the carry flag and clears the N flag and H flag
    void daA(); //complicata leggere OBSIDIAN
    void cpA(); //flips all bits in A, sets N and H flags

    //aritmetiche e logiche 16 bit
    void incR16(uint16_t *operand); //reads R and does valueof(R) + 1
    void decR16(uint16_t *operand); //reads R and does valueof(R) - 1
    void addHlR(uint16_t *operand); //does HL + R and stores in HL
    void addSpE(); //does Sp + E and stores in Sp. (E is 8 bit)
    
    void rlca();
    void rrca();
    void rla();
    void rra();
    void rlcR(uint8_t *operand); //da vedere se 8 o 16 bit
    void rlcHl();
    void rrcR(uint8_t *operand); //same
    void rrcHl();
    void rrR(uint8_t *operand); //same
    void rrHl();
    void slaR(uint8_t *operand); //same
    void slaHl();
    void sraR(uint8_t *operand); //same
    void sraHl();
    void swapR(uint8_t *operand); //same
    void swapHl();
    void srlR(uint8_t *operand); //same
    void srlHl();
    void bitBR(uint8_t *operand); //same
    void bitBHl();
    void resBR(uint8_t *operand); //same
    void resBHl();
    void setBR(uint8_t *operand); //same
    void setBHl();
};
#include <stdint.h>

enum RegistersFlags{
    ZERO_FLAG = (1<<7), //Z flag
    SUBTRACTION_FLAG = (1<<6), //N flag
    HALF_CARRY_FLAG = (1<<5), //H flag
    CARRY_FLAG = (1<<4) //C flag
};

class Registers {

    public:
    uint16_t pc = 0; //program counter
    uint16_t sp; //stack-pointer
    struct {
        union {
            struct {
                uint8_t f; //ha un valore che corrisponde alla flag se presente.
                uint8_t a;
            };
            uint16_t af;
        };
    };

    struct {
        union {
            struct {
                uint8_t c;
                uint8_t b;
            };
            uint16_t bc;
        };
    };

    struct {
        union {
            struct {
                uint8_t e;
                uint8_t d;
            };
            uint16_t de;
        };
    };

    struct {
        union {
            struct {
                uint8_t l;
                uint8_t h;
            };
            uint16_t hl;
        };
    };

    void setFlag(uint8_t flag);
    bool isFlagSet(uint8_t flag);
};
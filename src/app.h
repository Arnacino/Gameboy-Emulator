#ifndef APP_H
#define APP_H

class CPU;
class Memory;
class Registers;
class Interrupt;
class Instructions;
class PPU;
class SDLDisplay;
class Joypad;

class App {

private:
    SDLDisplay* display;
    CPU* cpu;
    Memory* memory;
    Registers* registers;
    Instructions* instructions;
    Interrupt* interrupt;
    PPU* ppu;
    Joypad* joypad;
public:
    App(const char* filepath);
    ~App();
    void run();
};

#endif
#ifndef APP_H
#define APP_H

class CPU;
class Memory;
class Registers;
class Interrupt;
class Instructions;

class App {

private:
    CPU* cpu;
    Memory* memory;
    Registers* registers;
    Instructions* instructions;
    Interrupt* interrupt;
public:
    App(const char* filepath);
    ~App();
    void run();
};

#endif
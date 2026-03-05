#ifndef INTERRUPT_H
#define INTERRUPT_H

class Interrupt
{
private:
    bool IME;
public:
    void enableIME(){
        this->IME = true;
    }

    void disableIME(){
        this->IME = false;
    }

    bool isIMEset(){
        return this->IME;
    }
};

#endif
#ifndef INTERRUPT_H
#define INTERRUPT_H

class Interrupt
{
private:
    bool IME;
public:
    void setIME(bool value){
        this->IME = value;
    }

    bool isIMEset(){
        return this->IME;
    }
};

#endif
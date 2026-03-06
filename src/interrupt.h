#ifndef INTERRUPT_H
#define INTERRUPT_H

class Interrupt
{
private:
    bool IME = false;
    bool pendingIME = false;
public:
    void enableIME(){
        this->IME = true;
    }

    void disableIME(){
        this->IME = false;
        this->pendingIME = false;
    }

    bool isIMEset(){
        return this->IME;
    }

    void scheduleEnableIME(){
        this->pendingIME = true;
    }

    void updateIME(){
        if(this->pendingIME){
            this->IME = true;
            this->pendingIME = false;
        }
    }

};

#endif
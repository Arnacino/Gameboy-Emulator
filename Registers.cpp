#include <Registers.h>

//ritorna 1 se la flag è settata 0 altrimenti
bool Registers::isFlagSet(uint8_t flag) {
    return this->f & flag; 
}

//aggiunge o rimuove una flag in base al valore
void Registers::setFlag(uint8_t flag){
    if(isFlagSet(flag)){
        f = ~flag;
    }else{
        f = flag;
    }
}
#include "app.h"

int main(int, char**){
    
    char filepath[] = "C:\\Users\\samue\\Desktop\\Progetti\\gb-test-roms\\mem_timing\\individual\\01-read_timing.gb";
    //char filepath[] = "C:\\Users\\samue\\Desktop\\Progetti\\gb-test-roms\\cpu_instrs\\cpu_instrs.gb";
    App* app = new App(filepath);
    app->run();
}

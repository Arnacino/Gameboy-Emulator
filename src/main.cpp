#include "app.h"

int main(int, char**){
    
    char filepath[] = "C:\\Users\\samue\\Desktop\\Progetti\\gb-test-roms\\mem_timing\\individual\\01-read_timing.gb";
    App* app = new App(filepath);
    app->run();
}

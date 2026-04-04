#include "app.h"

int main(int, char**){
    // char filepath[] = "roms/tests/instr_timing/instr_timing.gb"; // passed
    // char filepath[] = "roms/tests/cpu_instrs/cpu_instrs.gb"; // passed
    // char filepath[] = "roms/tests/oam_bug/rom_singles/3-non_causes.gb"; // passed
    // char filepath[] = "roms/tests/oam_bug/rom_singles/6-timing_no_bug.gb"; // passed

    // char filepath[] = "roms/tests/mem_timing/individual/01-read_timing.gb"; // failed
    // char filepath[] = "roms/tests/mem_timing/individual/02-write_timing.gb"; // failed
    // char filepath[] = "roms/tests/mem_timing/individual/03-modify_timing.gb"; // failed
    // char filepath[] = "roms/tests/mem_timing-2/rom_singles/01-read_timing.gb"; // failed
    // char filepath[] = "roms/tests/mem_timing-2/rom_singles/02-write_timing.gb"; // failed
    // char filepath[] = "roms/tests/mem_timing-2/rom_singles/03-modify_timing.gb"; // failed
    // char filepath[] = "roms/tests/interrupt_time/interrupt_time.gb"; // failed
    // char filepath[] = "roms/tests/oam_bug/rom_singles/1-lcd_sync.gb"; // failed
    // char filepath[] = "roms/tests/oam_bug/rom_singles/2-causes.gb"; // failed
    // char filepath[] = "roms/tests/oam_bug/rom_singles/4-scanline_timing.gb"; // failed
    // char filepath[] = "roms/tests/oam_bug/rom_singles/5-timing_bug.gb"; // failed
    // char filepath[] = "roms/tests/oam_bug/rom_singles/7-timing_effect.gb"; // failed
    // char filepath[] = "roms/tests/oam_bug/rom_singles/8-instr_effect.gb"; // failed

    char filepath[] = "roms/Dr. Mario (World).gb";
    // char filepath[] = "roms/Tetris (World) (Rev A).gb";

    App* app = new App(filepath);
    app->run();
}
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "memory/registers.h"
#include "cpu/interrupt.h"
#include "cpu/instructions.h"
#include "ppu/ppu.h"
#include "display/sdldisplay.h"
#include "joypad/joypad.h"

#include <chrono>
#include <thread>

int main(int, char**) {
    //char filepath[] = "roms/tests/instr_timing/instr_timing.gb"; // passed
    // char filepath[] = "roms/tests/cpu_instrs/cpu_instrs.gb"; // passed
    // char filepath[] = "roms/tests/oam_bug/rom_singles/3-non_causes.gb"; // passed
    // char filepath[] = "roms/tests/oam_bug/rom_singles/6-timing_no_bug.gb"; // passed

    char filepath[] = "roms/tests/mem_timing/individual/01-read_timing.gb"; // failed
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
    // char filepath[] = "roms/tests/halt_bug.gb"; // ?? idk

    //char filepath[] = "roms/Dr. Mario (World).gb";
    // char filepath[] = "roms/Tetris (World) (Rev A).gb";

    Joypad joypad;
    SDLDisplay display(&joypad);
    Memory memory(filepath, &joypad);
    Registers registers;
    Interrupt interrupt;
    Instructions instructions(&registers, &memory, &interrupt);
    CPU cpu(&memory, &registers, &instructions, &interrupt);
    PPU ppu(&memory);

    if (!display.init()) {
        return 1;
    }

    cpu.setRunning(true);

    static constexpr int FRAMETIME = 17556;
    bool shouldRun = true;
    int cycles = 0;

    using clock = std::chrono::steady_clock;
    const auto frameDuration = std::chrono::microseconds(16742);
    auto nextFrame = clock::now();

    while (shouldRun) {
        nextFrame += frameDuration;

        int frameCycles = 0;
        while (frameCycles < FRAMETIME) {
            cycles = cpu.step();
            frameCycles += cycles;
            ppu.update(cycles * 4);
        }

        display.setFrameBuffer(ppu.getFrameBuffer());
        shouldRun = display.loop();

        std::this_thread::sleep_until(nextFrame);
    }

    return 0;
}
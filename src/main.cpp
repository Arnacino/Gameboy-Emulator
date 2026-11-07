#include "cpu/cpu.h"
#include "memory/memory.h"
#include "memory/registers.h"
#include "cpu/interrupt.h"
#include "cpu/instructions.h"
#include "ppu/ppu.h"
#include "display/sdldisplay.h"
#include "joypad/joypad.h"
#include "rom/rom_selector.h"

#include <chrono>
#include <thread>

int main(int, char**) {
    std::string filepath = RomSelector::selectRom();
    if (filepath.empty()) {
        return 0;
    }

    Joypad joypad;
    SDLDisplay display(&joypad);
    Memory memory(filepath.c_str(), &joypad);
    Registers registers;
    Interrupt interrupt(&memory, &registers);
    Instructions instructions(&registers, &memory, &interrupt);
    CPU cpu(&memory, &registers, &instructions, &interrupt);
    PPU ppu(&memory);

    if (!display.init()) {
        return 1;
    }

    cpu.setRunning(true);

    static constexpr int FRAMETIME = 17556;
    bool shouldRun = true;

    using clock = std::chrono::steady_clock;
    const auto frameDuration = std::chrono::microseconds(16742);
    auto nextFrame = clock::now();

    while (shouldRun) {
        nextFrame += frameDuration;

        int frameMCycles = 0;
        while (frameMCycles < FRAMETIME) {
            cpu.step();
            ++frameMCycles;
            ppu.update(4);
        }
        display.setFrameBuffer(ppu.getFrameBuffer());
        shouldRun = display.loop();

        std::this_thread::sleep_until(nextFrame);
    }

    return 0;
}
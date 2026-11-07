cmake_minimum_required(VERSION 3.21)

project(Gameboy-Emulator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.5
)

FetchContent_MakeAvailable(SDL2)

add_library(emulator_lib
    src/cpu/cpu.cpp
    src/cpu/instructions.cpp
    src/cpu/interrupt.cpp
    src/memory/memory.cpp
    src/memory/registers.cpp
    src/joypad/joypad.cpp
    src/ppu/ppu.cpp
    src/display/sdldisplay.cpp
    src/rom/rom_selector.cpp
)

target_include_directories(emulator_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(emulator_lib
    PUBLIC
        SDL2-static
)

add_executable(GameboyEmulator
    src/main.cpp
)

target_link_libraries(GameboyEmulator
    PRIVATE
        emulator_lib
)
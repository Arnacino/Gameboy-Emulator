cmake_minimum_required(VERSION 3.14)

project(Gameboy-Emulator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Libreria con la logica dell'emulatore (senza main.cpp) ----
add_library(emulator_lib
    src/cpu.cpp
    src/instructions.cpp
    src/memory.cpp
    src/registers.cpp
    src/app.cpp
    src/ppu.cpp
)

# Esponi gli header della cartella src
target_include_directories(emulator_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ---- Eseguibile principale ----
add_executable(GameboyEmulator src/main.cpp)
target_link_libraries(GameboyEmulator PRIVATE emulator_lib)

# ---- Dipendenze esterne ----
include(FetchContent)

# Fetch SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.5
)
FetchContent_MakeAvailable(SDL2)

target_include_directories(GameboyEmulator PRIVATE ${SDL2_SOURCE_DIR}/include)
target_link_libraries(GameboyEmulator PRIVATE SDL2-static)
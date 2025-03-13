cmake_minimum_required(VERSION 3.10.0)

# Force CMake to use the specified compiler for all subprojects
set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe" CACHE STRING "" FORCE)
set(CMAKE_CXX_COMPILER "C:/ProgramData/mingw64/mingw64/bin/g++.exe" CACHE STRING "" FORCE)

project(Gameboy-Emulator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
file(GLOB SOURCES 
    "src/*.cpp"
    "src/*.h"
)

# Main executable
add_executable(GameboyEmulator ${SOURCES})

# Testing
Include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0
    CMAKE_ARGS
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
)

FetchContent_MakeAvailable(Catch2)

enable_testing()
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

# Add the test executable
add_executable(
    instructionsTests
    tests/instructionsTests.cpp
)
# Crea una libreria con i file sorgente
add_library(emulator_lib
    src/instructions.cpp
    src/memory.cpp
    src/registers.cpp
)

# Link la libreria all'eseguibile dei test
target_link_libraries(
    instructionsTests 
    PRIVATE 
    Catch2::Catch2WithMain
    emulator_lib
)

# Register tests
include(CTest)
include(Catch)
catch_discover_tests(instructionsTests)
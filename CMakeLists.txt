cmake_minimum_required(VERSION 3.10.0)
project(Gameboy-Emulator VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_COMPILER g++)
add_executable(GameboyEmulator src/main.cpp)

include(CTest)
enable_testing()


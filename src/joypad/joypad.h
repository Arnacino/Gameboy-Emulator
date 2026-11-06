#ifndef JOYPAD_H
#define JOYPAD_H

#include <array>
#include <cstdint>

enum class JoypadKey : uint8_t{
    Right = 0,
    Left,
    Up,
    Down,
    A,
    B,
    Select,
    Start
};

class Joypad
{
public:
    Joypad() = default;

    void setKey(JoypadKey key, bool pressed);
    uint8_t readP1(uint8_t p1SelectBits) const;

private:
    std::array<bool, 8> isKeyPressed{};
};

#endif
#include "joypad.h"
#include <cstddef>

void Joypad::setKey(JoypadKey key, bool pressed)
{
    isKeyPressed[static_cast<size_t>(key)] = pressed;
}

uint8_t Joypad::readP1(uint8_t p1SelectBits) const
{
    uint8_t value = static_cast<uint8_t>(0xC0 | (p1SelectBits & 0x30) | 0x0F);

    const bool selectDirections = (value & 0x10) == 0;
    const bool selectButtons = (value & 0x20) == 0;

    if (selectDirections)
    {
        if (isKeyPressed[static_cast<size_t>(JoypadKey::Right)])
            value &= static_cast<uint8_t>(~0x01);
        if (isKeyPressed[static_cast<size_t>(JoypadKey::Left)])
            value &= static_cast<uint8_t>(~0x02);
        if (isKeyPressed[static_cast<size_t>(JoypadKey::Up)])
            value &= static_cast<uint8_t>(~0x04);
        if (isKeyPressed[static_cast<size_t>(JoypadKey::Down)])
            value &= static_cast<uint8_t>(~0x08);
    }

    if (selectButtons)
    {
        if (isKeyPressed[static_cast<size_t>(JoypadKey::A)])
            value &= static_cast<uint8_t>(~0x01);
        if (isKeyPressed[static_cast<size_t>(JoypadKey::B)])
            value &= static_cast<uint8_t>(~0x02);
        if (isKeyPressed[static_cast<size_t>(JoypadKey::Select)])
            value &= static_cast<uint8_t>(~0x04);
        if (isKeyPressed[static_cast<size_t>(JoypadKey::Start)])
            value &= static_cast<uint8_t>(~0x08);
    }

    return value;
}
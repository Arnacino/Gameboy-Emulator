#include <cstdint>

enum class PPUMode : uint8_t {
    HBlank = 0,      // Mode 0
    VBlank = 1,      // Mode 1
    OamScan = 2,     // Mode 2
    Drawing = 3      // Mode 3
};

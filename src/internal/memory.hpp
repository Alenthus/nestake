#ifndef NESTAKE_MEMORY
#define NESTAKE_MEMORY

#include <array>
#include <stdint.h>

namespace nestake {
    class Memory {
    public:
        std::array<uint8_t, 2048> RAM;
        uint8_t Read(uint16_t address);
        void Write(uint16_t address, uint8_t value);
    };
}
#endif

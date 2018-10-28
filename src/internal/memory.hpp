#ifndef NESTAKE_MEMORY
#define NESTAKE_MEMORY

#include <vector>

namespace nestake {
    class Memory {
    public:
        std::vector<uint8_t> RAM;

        uint8_t Read(uint16_t address);
        void Write(uint16_t address, uint8_t value);
    };
}
#endif

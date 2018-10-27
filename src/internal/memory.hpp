#ifndef NESTAKE_MEMORY
#define NESTAKE_MEMORY

#include <map>

namespace nestake {
    class Memory {
    public:
        uint8_t Read(uint16_t address);
        void Write(uint16_t address, uint8_t value);
    };
}
#endif

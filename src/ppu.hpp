
#include "cpu.hpp"
#include "memory.hpp"

namespace nestake {

    class PPU {
    public:
        std::shared_ptr<PPUMemory> mem;
        std::shared_ptr<Cpu> cpu;

        void Reset();
    };
}
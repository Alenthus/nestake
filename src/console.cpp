#include "cpu.hpp"
#include "console.hpp"
#include <iostream>


namespace nestake {

    void Console::Step() {
        uint64_t cpuCycle = CPU->Step();
        std::cout << "cpu cycle: " << cpuCycle << "\n";

        // TODO: implement {PPU|Mapper|APU}.Step()
    }
}

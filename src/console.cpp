#include "cpu.hpp"
#include "console.hpp"
#include <iostream>


namespace nestake {

    void Console::Step() {
        uint64_t cpu_cycle = CPU->Step();
        std::cout << "cpu cycle: " << cpu_cycle << "\n";

        // TODO: implement {PPU|Mapper|APU}.Step()
    }
}

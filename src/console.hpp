#ifndef CONSOLE_CPU
#define CONSOLE_CPU

#include <utility>
#include "cpu.hpp"
#include "ines.hpp"

namespace nestake {
    class Console {
        std::shared_ptr<nestake::Cpu> CPU;
        std::shared_ptr<nestake::Cartridge> Cartridge;
        // PPU
        // APU
        // controller1
        // controller2
        // Mapper
    public:
        Console(std::shared_ptr<nestake::Cpu> cpu, std::shared_ptr<nestake::Cartridge> cartridge
        ): CPU(std::move(cpu)), Cartridge(std::move(cartridge)) {};

        // one step forward
        void Step();
    };
}

#endif
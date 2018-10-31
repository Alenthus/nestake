/*
 * implement memory Read/Write on memory.hpp 
 */

#include "memory.hpp"

namespace nestake {
    uint8_t Memory::Read(uint16_t address) {
        if (address < 0x2000) {
            return RAM[address%0x800];
        } else if (address < 0x4000) {
            // TODO: read from PPU
            return 0;
        } else if (address == 0x4014) {
            // TODO: read from PPU
            return 0;
        } else if (address == 0x4015) {
            // TODO: read from APU
            return 0;
        } else if (address == 0x4016) {
            // TODO: read from controller
            return 0;
        } else if (address == 0x4017) {
            // TODO: read from controller
            return 0;
        } else if (address > 0x4020) {
            // TODO: read from mapper
            return 0;
        }
        return 0;
    }

    void Memory::Write(uint16_t address, uint8_t value) {
        if (address < 0x2000) {
            RAM[address%0x0800] = value;
        } else if (address < 0x4000) {
            // TODO: write from PPU
        } else if (address == 0x4014) {
            // TODO: write from PPU
        } else if (address == 0x4015) {
            // TODO: write from APU
        } else if (address == 0x4016) {
            // TODO: write from controller
        } else if (address == 0x4017) {
            // TODO: write from controller
        } else if (address >= 0x4020) {
            // TODO: write from mapper
        }
    }
}

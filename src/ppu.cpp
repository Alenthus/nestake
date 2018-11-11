#include "ppu.hpp"

namespace nestake {

    void PPU::Reset() {
        Cycle = 340;
        ScanLine = 240;
        Frame = 0;
        writeControl(0);
        writeMask(0);
        writeOAMAddress(0);
    }


    PPU::PPU() {
        // TODO:
    }

    uint8_t PPU::ReadPalette(uint16_t address) {
        if (address >= 0x10 && address % 4 == 0) {
            address -= 16;
        }
        return paletteData[address];
    }

    void PPU::WritePalette(uint16_t address, uint8_t value) {
        if (address >= 0x10 && address % 4 == 0) {
            address -= 16;
        }
        paletteData[address] = value;
    }

    void PPU::writeOAMAddress(uint8_t v) {
            oamAddress = v;
    }

    void PPU::writeOAMData(uint8_t v) {
        oamData[oamAddress] = v;
        ++oamAddress;
    }

    // 0x2000
    void PPU::writeControl(uint8_t v) {

    };

    void PPU::writeMask(uint8_t v) {
        flagGrayscale = v & uint8_t(1);
        flagShowLeftBackground = (v >> 1) & (uint8_t(1));
        flagShowLeftSprites = (v >> 2) & (uint8_t(1));
        flagShowBackground = (v >> 3) & (uint8_t(1));
        flagShowSprites = (v >> 4) & (uint8_t(1));
        flagRedTint = (v >> 5) & (uint8_t(1));
        flagGreenTint = (v >> 6) & (uint8_t(1));
        flagBlueTint = (v >> 7) & (uint8_t(1));
    };


}
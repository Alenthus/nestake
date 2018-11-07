
#include <iostream>
#include <fstream>
#include "ines.hpp"

// Ref: https://wiki.nesdev.com/w/index.php/INES
// 4e45 531a 0201 0100 0000 0000 0000 0000
namespace nestake {

    // check magic number from header
    void checkMagicNumber(const uint8_t m[4]) {
        bool is_valid = true;
        if ((m[0] != 0x4e) |
            (m[1] != 0x45) |
            (m[2] != 0x53) |
            (m[3] != 0x1a)) {
            is_valid = false;
        }
        if (!is_valid) {
            fputs ("File error", stderr); exit (1);
        }
    }

    Cartridge::Cartridge(std::string path) {
        FILE *f = std::fopen(path.c_str(), "rb");
        if (f == nullptr) {
            fputs ("File error", stderr); exit (1);
        }

        // read magic number
        uint8_t nesMagic[4]={};
        std::fread(nesMagic, 1, 4, f);
        checkMagicNumber(nesMagic);

        // Size of PRG ROM in 16 KB units
        uint8_t  numPRG;
        std::fread(&numPRG, 1, 1, f);
        if (numPRG == 0) {
            fputs ("File error", stderr); exit (1);
        }

        // Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
        uint8_t  numCHR;
        std::fread(&numCHR, 1, 1, f);
        if (numCHR == 0) {
            fputs ("File error", stderr); exit (1);
        }

        // flag 6
        uint8_t  control1;
        std::fread(&control1, 1, 1, f);

        // flag 7
        uint8_t  control2;
        std::fread(&control2, 1, 1, f);

        // define mapper
        uint8_t mapper1 = control1 >> 4;
        uint8_t mapper2 = control2 >> 4;
        Mapper = mapper1 | (mapper2 << 1);

        mapper1 = control1 & uint8_t(1);
        mapper2 = (control1 >>3) & uint8_t(1);
        Mirror = mapper1 | (mapper2 << 1);

        // padding
        std::fread(new(uint8_t[8]), 1, 8, f);

        // read PRG
        PRG.reserve(size_t(0x4000*numPRG));
        for (auto i = 0; i < size_t(0x4000*numPRG); ++i ) {
            std::fread(&PRG[i], 1, 1, f);
        }

        // read CHR
        CHR.reserve(size_t(0x4000*numCHR));
        for (auto i = 0; i < size_t(0x4000*numPRG); ++i ) {
            std::fread(&CHR[i], 1, 1, f);
        }
    }
}

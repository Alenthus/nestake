
#include <iostream>
#include "ines.hpp"

// Ref: https://wiki.nesdev.com/w/index.php/INES

namespace nestake {
    const uint32_t iNESMagicNumber = 0x1a53454e;

    Cartridge::Cartridge(std::string path) {
        FILE *f = std::fopen(path.c_str(), "rb");
        if (f == nullptr) {
            fputs ("File error", stderr); exit (1);
        }

        inesHeader h = inesHeader{};

        std::fread(&h, 1, 16, f);
        // read header
        std::cout << "h.magic: " << int(h.magic) << "\n";
    }


    // TODO: implement Save/Load
    void Cartridge::Save() {};
    void Cartridge::Load() {};
}
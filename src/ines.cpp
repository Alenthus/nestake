
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
        uint8_t nesMagic[8]={};

        std::fread(nesMagic, 1, 8, f);

        // read header
        std::cout << "\n\n\nh.magic: " << nesMagic[0] << "\n";
        std::cout << "\n\n\nh.magic: " << nesMagic[1] << "\n";
        std::cout << "\n\n\nh.magic: " << nesMagic[2] << "\n";
        std::cout << "\n\n\nh.magic: " << nesMagic[7] << "\n";

    }


    // TODO: implement Save/Load
    void Cartridge::Save() {};
    void Cartridge::Load() {};
}
#ifndef NESTAKE_INES
#define NESTAKE_INES

#include <memory>
#include <string>
#include <vector>

namespace nestake{
    class Cartridge {
    public:
        std::vector<uint8_t> PRG;
        std::vector<uint8_t> CHR;
        std::vector<uint8_t> SRAM;
        uint8_t Mapper;
        uint8_t Mirror;
        explicit Cartridge(std::string);
    };
}

#endif

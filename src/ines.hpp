#ifndef NESTAKE_INES
#define NESTAKE_INES

#include <memory>
#include <string>
#include <vector>
namespace nestake{

    struct inesHeader {
        uint32_t magic;
        uint8_t  numPRG;
        uint8_t  numCHR;
        uint8_t  control1;
        uint8_t  control2;
        uint8_t  numRAM;
        uint8_t  padding[7];
    };

    class Cartridge {
    public:
        std::vector<uint8_t> PRG;
        std::vector<uint8_t> CHR;
        std::vector<uint8_t> SRAM;
        uint8_t Mapper;
        uint8_t Mirror;
        uint8_t Battery;
        explicit Cartridge(std::string);
        void Save();
        void Load();
    };

}

#endif

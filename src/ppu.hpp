#ifndef NESTAKE_PPU
#define NESTAKE_PPU

#include "cpu.hpp"
#include "memory.hpp"

namespace nestake {

    struct color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    class PPU {
    private:
        std::shared_ptr<PPUMemory> mem;
        std::shared_ptr<Cpu> cpu;
        std::array<color, 64> palatte;
    public:
        PPU();

        // counters
        uint64_t Cycle;
        uint64_t ScanLine;
        uint64_t Frame;

        // register I/O
        uint8_t ReadRegister(uint16_t);
        void WriteRegister(uint16_t address, uint8_t value);

        // platte I/O
        uint8_t ReadPalette(uint16_t);
        void WritePalette(uint16_t address, uint8_t value);
        void Reset();

        // storage
        std::array<uint8_t, 32> paletteData;
        std::array<uint8_t, 2048> nameTableData;
        std::array<uint8_t, 256> oamData;

        // used for actual displaying
        std::array<color, 256*240> currentImage;

        // used for rendering during vblank timing
        std::array<color, 256*240> renderingImage;

        // NMI flags
        bool nmiOccurred;
        bool nmiOutput;
        bool nmiPrevious;
        uint8_t nmiDelay;

        // 0x2000 PPUCTRL
        uint8_t flagNameTable; // 0: 0x2000; 1: 0x2400; 2: 0x2800; 3: 0x2C00
        uint8_t flagIncrement; // 0: add 1; 1: add 32
        uint8_t flagSpriteTable; // 0: 0x0000; 1: 0x1000; ignored in 8x16 mode
        uint8_t flagBackgroundTable; // 0: 0x0000; 1: 0x1000
        uint8_t flagSpriteSize; // 0: 8x8; 1: 8x16
        uint8_t flagMasterSlave; // 0: read EXT; 1: write EXT
        void writeControl(uint8_t v);

        // 0x2001 PPUMASK
        uint8_t flagGrayscale         ;  // 0: color; 1: grayscale
        uint8_t flagShowLeftBackground;  // 0: hide; 1: show
        uint8_t flagShowLeftSprites   ;  // 0: hide; 1: show
        uint8_t flagShowBackground    ;  // 0: hide; 1: show
        uint8_t flagShowSprites       ;  // 0: hide; 1: show
        uint8_t flagRedTint           ;  // 0: normal; 1: emphasized
        uint8_t flagGreenTint         ;  // 0: normal; 1: emphasized
        uint8_t flagBlueTint          ;  // 0: normal; 1: emphasized
        void writeMask(uint8_t v);

        // 0x2002 PPUSTATUS
        uint8_t flagSpriteZeroHit;
        uint8_t flagSpriteOverflow;

        // OAMADDR for 0x2003 / 0x2004
        uint8_t oamAddress;
        void writeOAMAddress(uint8_t v);  // 0x2003
        void writeOAMData(uint8_t v);     // 0x2004

        // 0x2007 PPUDATA
        uint8_t bufferedData; // for buffered reads

        // miscellaneous
        uint16_t v; // current vram address (15 bit)
        uint16_t t; // temporary vram address (15 bit)
        uint8_t x; // fine x scroll (3 bit)
        uint8_t w; // write toggle (1 bit)
        uint8_t f; // even/odd frame flag (1 bit)
    };
}
#endif
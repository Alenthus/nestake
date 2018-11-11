#ifndef NESTAKE_CPU
#define NESTAKE_CPU

#include <array>
#include <functional>
#include <map>
#include <memory>
#include <stdint.h>
#include <string>

#include "memory.hpp"

namespace nestake {
    class Cpu {
    private:
        // struct consisting of information for instruction execution
        struct instructionParams {
            // instruction's id
            uint8_t ID;

            // addressing mode
            uint8_t AddressingMode;

            // the size of each instruction in byte
            uint8_t InstructionSizes;

            // the number of cycles used by the instruction
            uint8_t InstructionCycle;

            // the number of page cycles used by the instruction
            uint8_t PageCycle;

            std::function<void (uint16_t, bool)> executor;
        };

        // map of all instructions
        std::map<uint8_t , instructionParams> instructionTable;
        std::map<int, std::string> idToInstructionName;
    public:
        // flag related
        uint8_t getFlag();
        void setZ(uint8_t v);
        void setN(uint8_t v);
        void setZN(uint8_t v);
        void compare(uint8_t a, uint8_t b);
        void setFlags(uint8_t flag);

        // interruption related methods
        void irq();
        void nmi();

        // memory push related
        void push(uint8_t);
        void push16(uint16_t);

        // memory pull related
        uint8_t pull();
        uint16_t pull16();

        // memory related
        uint16_t read16(uint16_t);
        uint16_t read16Bug(uint16_t);

        // debug flag
        bool IsDebugMode;

        // pointer to CPUMemory
        std::shared_ptr<CPUMemory> mem;

        // cpu cycles
        uint64_t Cycles;

        // program counter
        uint16_t PC;

        // stack pointer
        uint8_t SP;

        // accumulator
        uint8_t A;

        // x register
        uint8_t X;

        // y register
        uint8_t Y;

        // c register
        uint8_t C;

        // zero flag
        uint8_t Z;

        // interrupt disable flag
        uint8_t I;

        // decimal mode flag
        uint8_t D;

        //break command flag
        uint8_t B;

        // unused flag
        uint8_t U;

        // overflow flag
        uint8_t V;

        // negative flag
        uint8_t N;

        // interrupt type to execute
        uint8_t Interrupt;

        // number of cycles to stall
        int Stall;

        // core method for executing instructions
        uint64_t Step();

        // interruption related methods
        void Reset();
        void TriggerIRQ();
        void TriggerNMI();

        // instructions
        void ExecADC(uint16_t, bool);
        void ExecAND(uint16_t, bool);
        void ExecASL(uint16_t, bool);
        void ExecBCC(uint16_t, bool);
        void ExecBCS(uint16_t, bool);
        void ExecBEQ(uint16_t, bool);
        void ExecBIT(uint16_t, bool);
        void ExecBMI(uint16_t, bool);
        void ExecBNE(uint16_t, bool);
        void ExecBPL(uint16_t, bool);
        void ExecBRK(uint16_t, bool);
        void ExecBVC(uint16_t, bool);
        void ExecBVS(uint16_t, bool);
        void ExecCLC(uint16_t, bool);
        void ExecCLD(uint16_t, bool);
        void ExecCLI(uint16_t, bool);
        void ExecCLV(uint16_t, bool);
        void ExecCMP(uint16_t, bool);
        void ExecCPX(uint16_t, bool);
        void ExecCPY(uint16_t, bool);
        void ExecDEC(uint16_t, bool);
        void ExecDEX(uint16_t, bool);
        void ExecDEY(uint16_t, bool);
        void ExecEOR(uint16_t, bool);
        void ExecINC(uint16_t, bool);
        void ExecINX(uint16_t, bool);
        void ExecINY(uint16_t, bool);
        void ExecJMP(uint16_t, bool);
        void ExecJSR(uint16_t, bool);
        void ExecLDA(uint16_t, bool);
        void ExecLDX(uint16_t, bool);
        void ExecLDY(uint16_t, bool);
        void ExecLSR(uint16_t, bool);
        void ExecORA(uint16_t, bool);
        void ExecPHA(uint16_t, bool);
        void ExecPHP(uint16_t, bool);
        void ExecPLA(uint16_t, bool);
        void ExecPLP(uint16_t, bool);
        void ExecROL(uint16_t, bool);
        void ExecROR(uint16_t, bool);
        void ExecRTI(uint16_t, bool);
        void ExecRTS(uint16_t, bool);
        void ExecSBC(uint16_t, bool);
        void ExecSEC(uint16_t, bool);
        void ExecSED(uint16_t, bool);
        void ExecSEI(uint16_t, bool);
        void ExecSTA(uint16_t, bool);
        void ExecSTX(uint16_t, bool);
        void ExecSTY(uint16_t, bool);
        void ExecTAX(uint16_t, bool);
        void ExecTAY(uint16_t, bool);
        void ExecTSX(uint16_t, bool);
        void ExecTXA(uint16_t, bool);
        void ExecTXS(uint16_t, bool);
        void ExecTYA(uint16_t, bool);

        // setup (instruction table)
        explicit Cpu(std::shared_ptr<CPUMemory>);
    };

    bool isPageCrossed(uint16_t, uint16_t);
}
#endif

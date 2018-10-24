#ifndef NESTAKE_CPU
#define NESTAKE_CPU

#include <array>
#include <map>
#include <string>
#include "memory.hpp"

class NesCpu {
public:
    // pointer to NesMemory interface
    NesMemory* Memory;

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
    uint8_t interrupt;

    // number of cycles to stall
    int stall;

    // core method for executing instructions
    int Step();

    // interruption related methods
    void TriggerIRQ();
    void TriggerNMI();
    void irq();
    void nmi();

    // memory related
    uint16_t Read16(uint16_t);
};

// interface of instruction executor
typedef void (*InstructionExecutor) (NesCpu* cpu, uint16_t address, uint16_t pc, bool is_accumulator_mode);

// struct consisting of information for instruction execution
struct InstructionParams {
    // instruction's name ... ex.) NOP
    std::string InstructionName;

    // addressing mode ... ex.) Zeropage
    uint8_t AddressingMode;

    // the size of each instruction in byte
    uint8_t InstructionSizes;

    // the number of cycles used by the instruction
    uint8_t InstructionCycle;

    // the number of page cycles used by the instruction
    uint8_t PageCycle;

    InstructionExecutor executor;
};
#endif

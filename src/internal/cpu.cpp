#include "cpu.hpp"
#include <string>
#include "memory.cpp"

// Addressing mode struct for expressing each addressing mode as int

using std::array;
using std::map;
using std::string;

enum InstructionID {
    ADC = 1, AHX, ALR, ANC, AND, ARR, ASL, AXS, BCC, BCS, BEQ, BIT, BMI,
    BNE, BPL, BRK, BVC, BVS, CLC, CLD, CLI, CLV, CMP, CPX, CPY, DCP, DEC,
    DEX, DEY, EOR, INC, INX, INY, ISC, JMP, JSR, KIL, LAS, LAX, LDA, LDX,
    LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, RLA, ROL, ROR, RRA, RTI, RTS,
    SAX, SBC, SEC, SED, SEI, SHX, SHY, SLO, SRE, STA, STX, STY, TAS, TAX,
    TAY, TSX, TXA, TXS, TYA, XAA,
};

map<uint8_t, string> idToInstructionName = {
        {ADC, "ADC"}, {AHX, "AHX"}, {ALR, "ALR"}, {ANC, "ANC"}, {AND, "AND"}, {ARR, "ARR"}, {ASL, "ASL"}, {AXS, "AXS"}, {BCC, "BCC"}, {BCS, "BCS"}, {BEQ, "BEQ"}, {BIT, "BIT"}, {BMI, "BMI"}, {BNE, "BNE"}, {BPL, "BPL"}, {BRK, "BRK"},
        {BVC, "BVC"}, {BVS, "BVS"}, {CLC, "CLC"}, {CLD, "CLD"}, {CLI, "CLI"}, {CLV, "CLV"}, {CMP, "CMP"}, {CPX, "CPX"}, {CPY, "CPY"}, {DCP, "DCP"}, {DEC, "DEC"}, {DEX, "DEX"}, {DEY, "DEY"}, {EOR, "EOR"}, {INC, "INC"}, {INX, "INX"},
        {INY, "INY"}, {ISC, "ISC"}, {JMP, "JMP"}, {JSR, "JSR"}, {KIL, "KIL"}, {LAS, "LAS"}, {LAX, "LAX"}, {LDA, "LDA"}, {LDX, "LDX"}, {LDY, "LDY"}, {LSR, "LSR"}, {NOP, "NOP"}, {ORA, "ORA"}, {PHA, "PHA"}, {PHP, "PHP"}, {PLA, "PLA"},
        {PLP, "PLP"}, {RLA, "RLA"}, {ROL, "ROL"}, {ROR, "ROR"}, {RRA, "RRA"}, {RTI, "RTI"}, {RTS, "RTS"}, {SAX, "SAX"}, {SBC, "SBC"}, {SEC, "SEC"}, {SED, "SED"}, {SEI, "SEI"}, {SHX, "SHX"}, {SHY, "SHY"}, {SLO, "SLO"}, {SRE, "SRE"},
        {STA, "STA"}, {STX, "STX"}, {STY, "STY"}, {TAS, "TAS"}, {TAX, "TAX"}, {TAY, "TAY"}, {TSX, "TSX"}, {TXA, "TXA"}, {TXS, "TXS"}, {TYA, "TYA"}, {XAA, "XAA"}
};


enum AddressingMode {
    Absolute = 1, AbsoluteX, AbsoluteY, Accumulator,
    Immediate, Implied, IndexedIndirect, Indirect,
    IndirectIndexed, Relative, ZeroPage, ZeroPageX,
    ZeroPageY
};

// type of interruption
enum InterruptType {
    interruptNone = 1,
    interruptNMI,
    interruptIRQ,
};

bool isPageDiffer(uint16_t a, uint16_t b) {
    // TODO: implement
    return true;
}


// TODO: implement all instructions and append the following InstructionTable array

// Array of instruction params to be selected by its corresponding bits.
array<InstructionParams*, 256> InstructionTable = {};

uint16_t NesCpu::Read16(uint16_t address) {
    uint16_t lo = Memory->Read(address);
    uint16_t hi = Memory->Read(address + uint16_t(1)) <<8;
    return hi | lo;
}

uint16_t NesCpu::Read16Bug(uint16_t) {
    // TODO: implement
    return 0;
}

void NesCpu::TriggerIRQ() {
    interrupt = interruptIRQ;
}

void NesCpu::TriggerNMI() {
    interrupt = interruptNMI;
}


// TODO: implement
void NesCpu::irq() {}

void NesCpu::nmi() {}

uint64_t NesCpu::Step() {

    // stall cpu cycle
    if (stall > 0) {
        --stall;
        return 1;
    }

    switch (interrupt) {
        case interruptNMI:
            nmi();
            break;
        case interruptIRQ:
            irq();
            break;
        default:{}
    }
    // reset interrupt flag
    interrupt = interruptNone;

    // auxiliary variables
    uint64_t prev_cycles = Cycles;
    uint16_t address = 0;
    bool page_crossed = false;
    InstructionParams* inst = InstructionTable[PC];

    // switching the instruction's condition depending on addressing mode
    switch (inst->AddressingMode) {
        case Absolute : {
            address = Read16(PC + uint16_t(1));
            break;
        }
        case AbsoluteX: {
            address = Read16(PC + uint16_t(1)) + uint8_t(X);
            page_crossed = isPageDiffer(address - uint8_t(X), address);
            break;
        }
        case AbsoluteY: {
            address = Read16(PC + uint16_t(1)) + uint8_t(Y);
            page_crossed = isPageDiffer(address - uint8_t(Y), address);
            break;
        }
        case Immediate: {
            address = PC + uint16_t(1);
            break;
        }
        case IndexedIndirect: {
            address = Read16Bug(Memory->Read(PC + uint16_t(1)) + uint16_t (X));
            break;
        }
        case Indirect: {
            address = Read16Bug(Read16(PC + uint16_t(1)));
            break;
        }
        case IndirectIndexed: {
            address = Read16Bug(Memory->Read(PC + uint16_t(1))) + uint16_t (Y);
            page_crossed = isPageDiffer(address -uint16_t(Y), address);
        }
        case Relative: {
            // TODO:
        }
        case ZeroPage: {
            address = Memory->Read(PC + uint16_t(1));
            break;
        }
        case ZeroPageX: {
            // TODO:
        }
        case ZeroPageY: {
            // TODO:
        }
        default: {}
    }

    PC += inst->InstructionSizes;
    Cycles += inst->InstructionCycle;
    if (page_crossed) {
        Cycles += inst->PageCycle;
    }

    inst->executor(this, address, PC, inst->AddressingMode == Accumulator);

    return Cycles - prev_cycles;
}
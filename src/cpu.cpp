#include <iostream>
#include <string>

#include "cpu.hpp"
#include "memory.cpp"

using std::array;
using std::cout;
using std::map;
using std::string;

namespace nestake {
    // addressing mode
    enum AddressingMode {
        Absolute = 1, AbsoluteX, AbsoluteY, Accumulator, Immediate,
        Implied, IndexedIndirect, Indirect, IndirectIndexed, Relative,
        ZeroPage, ZeroPageX, ZeroPageY
    };

    // type of interruption
    enum InterruptType {
        interruptNone = 1, interruptNMI, interruptIRQ,
    };

    // instructions's identifier supposed to be used in debugging
    enum InstructionID {
        ADC = 1, AHX, ALR, ANC, AND, ARR, ASL, AXS, BCC, BCS, BEQ, BIT, BMI,
        BNE, BPL, BRK, BVC, BVS, CLC, CLD, CLI, CLV, CMP, CPX, CPY, DCP, DEC,
        DEX, DEY, EOR, INC, INX, INY, ISC, JMP, JSR, KIL, LAS, LAX, LDA, LDX,
        LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, RLA, ROL, ROR, RRA, RTI, RTS,
        SAX, SBC, SEC, SED, SEI, SHX, SHY, SLO, SRE, STA, STX, STY, TAS, TAX,
        TAY, TSX, TXA, TXS, TYA, XAA,
    };

    bool isPageCrossed(uint16_t a, uint16_t b) {
        return (a&0xFF00) != (b&0xFF00);
    }

    // ADC instruction
    void Cpu::ExecADC(uint16_t address, bool){
        uint8_t a = A;
        uint8_t b = mem->Read(address);
        A = a + b + C;

        // set Z & N flags
        setZN(A);

        // set carry flag
        if ((int(a) + int(b) + int(C)) > 0xFF) {
            C = 1;
        } else {
            C = 0;
        }

        // set overflow flag
        if ((((a^b)&0x80) == 0) && (((a^A)&0x80) != 0)){
            V = 1;
        } else {
            V = 0;
        }
    }

    // AND instruction
    void Cpu::ExecAND(uint16_t address, bool){
        A = A & mem->Read(address);
        // set Z & N flags
        setZN(A);
    }

    // ASL instruction
    void Cpu::ExecASL(uint16_t address, bool is_accumulator){
        if (is_accumulator) {
            C = (A >> 7) & uint8_t(1);
            A <<= 1;
            setZN(A);
        } else {
            uint8_t v = mem->Read(address);
            C = (v >> 7) & uint8_t(1);
            v <<= 1;
            mem->Write(address, v);
            setZN(v);
        }
    }

    // BCC instruction
    void Cpu::ExecBCC(uint16_t address, bool){
        if (C == 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        };
    };

    // BCS instruction
    void Cpu::ExecBCS(uint16_t address, bool){
        if (C != 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    // BEQ instruction
    void Cpu::ExecBEQ(uint16_t address, bool){
        if (Z != 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    // BIT operation
    void Cpu::ExecBIT(uint16_t address, bool){
        uint8_t v = mem->Read(address);
        setZ(v & A);
        V = (v >> 6) & uint8_t(1);
        setN((v >> 7) & uint8_t(1));
    };

    // BMI params
    void Cpu::ExecBMI(uint16_t address, bool){
        if (N != 0){
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    // BNE operation
    void Cpu::ExecBNE(uint16_t address, bool){
        if (Z == 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    // BPL operation
    void Cpu::ExecBPL(uint16_t address, bool){
        if (N == 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    // BRK instruction
    void Cpu::ExecBRK(uint16_t address, bool is_accumulator){
        push16(PC);
        ExecPHP(address, is_accumulator);
        ExecSEI(address, is_accumulator);
        PC = read16(0xFFFE);
    };

    // BVC instruction
    void Cpu::ExecBVC(uint16_t address, bool){
        if (V == 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    // BVS instruction
    void Cpu::ExecBVS(uint16_t address, bool){
        if (V != 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    // CLC instruction
    void Cpu::ExecCLC(uint16_t, bool){
        C = 0;
    };

    // CLD instruction
    void Cpu::ExecCLD(uint16_t, bool){
        D = 0;
    };

    // CLI instruction
    void Cpu::ExecCLI(uint16_t, bool){
        I = 0;
    };

    // CLV instruction
    void Cpu::ExecCLV(uint16_t, bool){
        V = 0;
    };

    // CMP instruction
    void Cpu::ExecCMP(uint16_t address, bool){
        uint8_t v = mem->Read(address);
        compare(A, v);
    };

    // CPX instruction
    void Cpu::ExecCPX(uint16_t address, bool){
        uint8_t v = mem->Read(address);
        ExecCMP(X, v);
    };

    // CPY instruction
    void Cpu::ExecCPY(uint16_t address, bool){
        uint8_t v = mem->Read(address);
        ExecCMP(Y, v);
    };

    // DEC instruction
    void Cpu::ExecDEC(uint16_t address, bool){
        uint8_t v = mem->Read(address) - uint8_t(1);
        mem->Write(address, v);
        setZN(v);
    };

    // DEX instruction
    void Cpu::ExecDEX(uint16_t, bool){
        --X;
        setZN(X);
    };

    // DEY instruction
    void Cpu::ExecDEY(uint16_t, bool){
        --Y;
        setZN(Y);
    };

    // EOR instruction
    void Cpu::ExecEOR(uint16_t address, bool){
        A = A ^ mem->Read(address);
        setZN(A);
    };

    // INC instruction
    void Cpu::ExecINC(uint16_t address, bool){
        uint8_t v = mem->Read(address) + uint8_t(1);
        mem->Write(address, v);
        setZN(v);
    };

    // INX instruction
    void Cpu::ExecINX(uint16_t, bool){
        ++X;
        setZN(X);
    };

    // INY instruction
    void Cpu::ExecINY(uint16_t, bool){
        ++Y;
        setZN(Y);
    };

    // JMP operation
    void Cpu::ExecJMP(uint16_t address, bool){
        PC = address;
    };

    // JSR operation
    void Cpu::ExecJSR(uint16_t address, bool){
        push16(PC - uint8_t(1));
        PC = address;
    };

    // LDA operation
    void Cpu::ExecLDA(uint16_t address, bool){
        A = mem->Read(address);
        setZN(A);
    };

    // LDX operation
    void Cpu::ExecLDX(uint16_t address, bool){
        X = mem->Read(address);
        setZN(X);
    };

    // LDY operation
    void Cpu::ExecLDY(uint16_t address, bool){
        Y = mem->Read(address);
        setZN(Y);
    };

    // LSR operation
    void Cpu::ExecLSR(uint16_t address, bool is_accumulator){
        if (is_accumulator) {
            C = A & uint8_t(1);
            A >>= 1;
            setZN(A);
        } else {
            uint8_t v = mem->Read(address);
            C = v & uint8_t(1);
            v >>= 1;
            mem->Write(address, v);
            setZN(v);
        }
    };

    // ORA operation
    void Cpu::ExecORA(uint16_t address, bool){
        A = A | mem->Read(address);
        setZN(A);
    };

    // PHA operation
    void Cpu::ExecPHA(uint16_t, bool){
        push(A);
    };

    // PHP operation
    void Cpu::ExecPHP(uint16_t, bool){
        push(getFlag()|uint8_t(0x10));
    };

    // PLA operation
    void Cpu::ExecPLA(uint16_t, bool){
        A = pull();
        setZN(A);
    };

    // PLP operation
    void Cpu::ExecPLP(uint16_t, bool){
        setFlags((pull()&uint8_t(0xEF))| uint8_t(0x20));
    };

    // ROL operation
    void Cpu::ExecROL(uint16_t address, bool is_accumulator){
        if (is_accumulator) {
            uint8_t prev_c = C;
            C = (A >> 7) & uint8_t(1);
            A = (A << 1) | prev_c;
            setZN(A);
        } else {
            uint8_t prev_c = C;
            uint8_t v = mem->Read(address);
            C = (v >> 7) & uint8_t(1);
            v = (v << 1) | prev_c;
            mem->Write(address, v);
            setZN(v);
        }
    };

    // ROR operation
    void Cpu::ExecROR(uint16_t address, bool is_accumulator){
        if (is_accumulator) {
            uint8_t prev_c = C;
            C = A & uint8_t(1);
            A = (A >> 1) | (prev_c << 7);
            setZN(A);
        } else {
            uint8_t prev_c = C;
            uint8_t v = mem->Read(address);
            C = v & uint8_t(1);
            v = (v >> 1) | (prev_c << 7);
            mem->Write(address, v);
            setZN(v);
        }
    };

    // RTI operation
    void Cpu::ExecRTI(uint16_t, bool){
        setFlags((pull()&uint8_t(0xEF))| uint8_t(0x20));
        PC = pull16();
    };

    // RTS operation
    void Cpu::ExecRTS(uint16_t, bool){
        PC = pull16() + uint16_t(1);
    };

    // SBC operation
    void Cpu::ExecSBC(uint16_t address, bool){
        uint8_t prev_a = A;
        uint8_t b = mem->Read(address);
        uint8_t prev_c = C;

        A = prev_a - b - (uint8_t(1) - prev_c);
        setZN(A);

        if ((int(prev_a) - int(b) - (1 - prev_c)) >= 0) {
            C = 1;
        } else {
            C = 0;
        }

        if ((((prev_a^b)&uint8_t(0x80)) != 0) && ((prev_a^A)&uint8_t(0x80))!= 0) {
            V = 1;
        } else {
            V = 0;
        }
    };

    // SEC operation
    void Cpu::ExecSEC(uint16_t, bool){
        C = 1;
    };

    // SED operation
    void Cpu::ExecSED(uint16_t, bool){
        D = 1;
    };

    // SEI operation
    void Cpu::ExecSEI(uint16_t, bool){
        I = 1;
    };

    // STA operation
    void Cpu::ExecSTA(uint16_t address, bool){
        mem->Write(address, A);
    };

    // STX operation
    void Cpu::ExecSTX(uint16_t address, bool){
        mem->Write(address, X);
    };

    // STY operation
    void Cpu::ExecSTY(uint16_t address, bool){
        mem->Write(address, Y);
    };

    // TAX operation
    void Cpu::ExecTAX(uint16_t, bool){
        X = A;
        setZN(X);
    };

    // TAY operation
    void Cpu::ExecTAY(uint16_t, bool){
        Y = A;
        setZN(Y);
    };

    // TSX operation
    void Cpu::ExecTSX(uint16_t, bool){
        X = SP;
        setZN(X);
    };


    // TXA operation
    void Cpu::ExecTXA(uint16_t, bool){
        A = X;
        setZN(A);
    };

    // TXS operation
    void Cpu::ExecTXS(uint16_t, bool){
        SP = X;
    };


    // TYA operation
    void Cpu::ExecTYA(uint16_t, bool){
        A = Y;
        setZN(A);
    };

    uint16_t Cpu::read16(uint16_t address) {
        uint16_t lo = mem->Read(address);
        uint16_t hi = mem->Read(address + uint16_t(1)) <<8;
        return hi | lo;
    }

    uint16_t Cpu::read16Bug(uint16_t address) {
        uint16_t _address = (address&uint16_t(0xFF00)) | uint16_t(address+1) <<8;
        uint16_t lo = mem->Read(address);
        uint16_t hi = mem->Read(_address) <<8;
        return hi | lo;
    }

    void Cpu::push(uint8_t v) {
        mem->Write(uint16_t(0x0100)|uint16_t(SP), v);
        --SP;
    }

    void Cpu::push16(uint16_t v) {
        push(uint8_t(v >> 8));
        push(uint8_t(v & 0xFF));
    }

    void Cpu::setN(uint8_t v) {
        if (v != 0) {
            N = 1;
        } else {
            N = 0;
        }
    }

    void Cpu::setZ(uint8_t v) {
        if (v == 0) {
            Z = 1;
        } else {
            Z = 0;
        }
    }

    void Cpu::setZN(uint8_t v) {
        setZ(v);
        setN(v);
    }

    void Cpu::compare(uint8_t a, uint8_t b) {
        setZN(a-b);
        if (a >= b) {
            C = 1;
        } else {
            C = 0;
        }
    }

    void Cpu::setFlags(uint8_t flag) {
        C = flag & uint8_t(1);
        Z = (flag >> 1) & uint8_t(1);
        I = (flag >> 2) & uint8_t(1);
        D = (flag >> 3) & uint8_t(1);
        B = (flag >> 4) & uint8_t(1);
        U = (flag >> 5) & uint8_t(1);
        V = (flag >> 6) & uint8_t(1);
        N = (flag >> 7) & uint8_t(1);
    }


    uint8_t Cpu::getFlag() {
        uint8_t flag = C;
        flag |= Z << 1;
        flag |= I << 2;
        flag |= D << 3;
        flag |= B << 4;
        flag |= U << 5;
        flag |= V << 6;
        flag |= N << 7;
        return flag;
    }

    uint8_t Cpu::pull() {
        ++SP;
        return mem->Read(uint16_t(0x100)|uint16_t(SP));
    }

    uint16_t Cpu::pull16() {
        uint16_t lo = pull();
        uint16_t hi = pull();
        return hi << 8 | lo;
    }

    void Cpu::Reset() {
        Cycles = 0;
        A = 0;
        X = 0;
        Y = 0;
        C = 0;
        Z = 0;
        I = 0;
        D = 0;
        B = 0;
        U = 0;
        V = 0;
        N = 0;
        Interrupt = 0;
        Stall = 0;

        PC = mem->Read(0xFFFC);
        SP = 0xFD;
        setFlags(0x24);
    }

    void Cpu::TriggerIRQ() {
        if (I == 0) {
            Interrupt = interruptIRQ;
        }
    }

    void Cpu::TriggerNMI() {
        Interrupt = interruptNMI;
    }


    void Cpu::irq() {
        push16(PC);
        ExecPHP(0, false);
        PC = read16(0xFFFE);
        I = 1;
        Cycles += 7;
    }

    void Cpu::nmi() {
        push16(PC);
        ExecPHP(0, false);
        PC = read16(0xFFFA);
        I = 1;
        Cycles += 7;
    }

    uint64_t Cpu::Step() {

        // stall cpu cycle
        if (Stall > 0) {
            --Stall;
            return 1;
        }

        switch (Interrupt) {
            case interruptNMI:
                nmi();
                break;
            case interruptIRQ:
                irq();
                break;
            default: {}
        }
        // reset interrupt flag
        Interrupt = interruptNone;

        // auxiliary variables
        uint64_t prev_cycles = Cycles;
        uint16_t address = 0;
        bool page_crossed = false;

        // read opcode
        uint8_t op = mem->Read(PC);
        InstructionParams inst = InstructionTable[op];

        // switch the instruction's condition according to the addressing mode
        switch (inst.AddressingMode) {
            case Absolute : {
                address = read16(PC + uint16_t(1));
                break;
            }
            case AbsoluteX: {
                address = read16(PC + uint16_t(1)) + uint8_t(X);
                page_crossed = isPageCrossed(address - uint8_t(X), address);
                break;
            }
            case AbsoluteY: {
                address = read16(PC + uint16_t(1)) + uint8_t(Y);
                page_crossed = isPageCrossed(address - uint8_t(Y), address);
                break;
            }
            case Immediate: {
                address = PC + uint16_t(1);
                break;
            }
            case IndexedIndirect: {
                address = read16Bug(mem->Read(PC + uint16_t(1)) + uint16_t(X));
                break;
            }
            case Indirect: {
                address = read16Bug(read16(PC + uint16_t(1)));
                break;
            }
            case IndirectIndexed: {
                address = read16Bug(mem->Read(PC + uint16_t(1))) + uint16_t(Y);
                page_crossed = isPageCrossed(address - uint16_t(Y), address);
                break;
            }
            case Relative: {
                uint16_t offset = mem->Read(PC + uint16_t(1));
                if (offset < 0x80) {
                    address = PC + uint16_t(2) + offset;
                } else {
                    address = PC + uint16_t(2) + offset - uint16_t(0x100);
                }
                break;
            }
            case ZeroPage: {
                address = mem->Read(PC + uint16_t(1));
                break;
            }
            case ZeroPageX: {
                address = uint16_t((mem->Read(PC + uint16_t(1)) + X) & 0xff);
                break;
            }
            case ZeroPageY: {
                address = uint16_t((mem->Read(PC + uint16_t(1)) + Y) & 0xff);
                break;
            }
            default: {
            }
        }

        PC += inst.InstructionSizes;
        Cycles += inst.InstructionCycle;
        if (page_crossed) {
            Cycles += inst.PageCycle;
        }

        inst.executor(address, inst.AddressingMode == Accumulator);

        if (IsDebugMode) {
            cout << "[Instruction]:" << idToInstructionName.find(inst.ID)->second;
            cout << "[address]: " << address;
            cout << "[addressing mode]: " << inst.AddressingMode << "\n";
        }

        return Cycles - prev_cycles;
    }

    // [&](uint16_t a, bool b){_php(a, b);}
    Cpu::Cpu(std::shared_ptr<Memory> m) {
        // setup instruction table
        // ref: http://pgate1.at-ninja.jp/NES_on_FPGA/nes_cpu.htm#instruction
        InstructionTable = {
            // ADC
            {0x69, {ADC, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecADC(a,b);}}}, {0x65, {ADC, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecADC(a,b);}}},
            {0x75, {ADC, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecADC(a,b);}}}, {0x6D, {ADC, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecADC(a,b);}}},
            {0x7D, {ADC, AbsoluteX, 3, 4, 1, [&](uint16_t a, bool b){ExecADC(a,b);}}}, {0x79, {ADC, AbsoluteY, 3, 4, 1, [&](uint16_t a, bool b){ExecADC(a,b);}}},
            {0x79, {ADC, IndexedIndirect, 2, 6, 0, [&](uint16_t a, bool b){ExecADC(a,b);}}}, {0x71, {ADC, IndirectIndexed, 2, 5, 1, [&](uint16_t a, bool b){ExecADC(a,b);}}},

            // SBC
            {0xE9, {SBC, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecSBC(a,b);}}}, {0xE5, {SBC, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecSBC(a,b);}}},
            {0xF5, {SBC, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecSBC(a,b);}}}, {0xED, {SBC, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecSBC(a,b);}}},
            {0xFD, {SBC, AbsoluteX, 3, 4, 1, [&](uint16_t a, bool b){ExecSBC(a,b);}}}, {0xF9, {SBC, AbsoluteY, 3, 4, 1, [&](uint16_t a, bool b){ExecSBC(a,b);}}},
            {0xE1, {SBC, IndexedIndirect, 2, 6, 0, [&](uint16_t a, bool b){ExecSBC(a,b);}}}, {0xF1, {SBC, IndirectIndexed, 2, 5, 1, [&](uint16_t a, bool b){ExecSBC(a,b);}}},

            // AND
            {0x29, {AND, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecAND(a,b);}}}, {0x25, {AND, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecAND(a,b);}}},
            {0x35, {AND, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecAND(a,b);}}}, {0x2D, {AND, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecAND(a,b);}}},
            {0x3D, {AND, AbsoluteX, 3, 4, 1, [&](uint16_t a, bool b){ExecAND(a,b);}}}, {0x39, {AND, AbsoluteY, 3, 4, 1, [&](uint16_t a, bool b){ExecAND(a,b);}}},
            {0x21, {AND, IndexedIndirect, 2, 6, 0, [&](uint16_t a, bool b){ExecAND(a,b);}}}, {0x31, {AND, IndirectIndexed, 2, 5, 1, [&](uint16_t a, bool b){ExecAND(a,b);}}},

            // ORA
            {0x09, {ORA, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecORA(a,b);}}}, {0x05, {ORA, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecORA(a,b);}}},
            {0x15, {ORA, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecORA(a,b);}}}, {0x0D, {ORA, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecORA(a,b);}}},
            {0x1D, {ORA, AbsoluteX, 3, 4, 1, [&](uint16_t a, bool b){ExecORA(a,b);}}}, {0x19, {ORA, AbsoluteY, 3, 4, 1, [&](uint16_t a, bool b){ExecORA(a,b);}}},
            {0x01, {ORA, IndexedIndirect, 2, 6, 0, [&](uint16_t a, bool b){ExecORA(a,b);}}}, {0x11, {ORA, IndirectIndexed, 2, 5, 1, [&](uint16_t a, bool b){ExecORA(a,b);}}},

            // EOR
            {0x49, {EOR, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecEOR(a,b);}}}, {0x45, {EOR, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecEOR(a,b);}}},
            {0x55, {EOR, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecEOR(a,b);}}}, {0x4D, {EOR, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecEOR(a,b);}}},
            {0x5D, {EOR, AbsoluteX, 3, 4, 1, [&](uint16_t a, bool b){ExecEOR(a,b);}}}, {0x59, {EOR, AbsoluteY, 3, 4, 1, [&](uint16_t a, bool b){ExecEOR(a,b);}}},
            {0x41, {EOR, IndexedIndirect, 2, 6, 0, [&](uint16_t a, bool b){ExecEOR(a,b);}}}, {0x51, {EOR, IndirectIndexed, 2, 5, 1, [&](uint16_t a, bool b){ExecEOR(a,b);}}},

            // ASL
            {0x0A, {ASL, Accumulator, 1, 2, 0, [&](uint16_t a, bool b){ExecASL(a,b);}}}, {0x06, {ASL, ZeroPage, 2, 5, 0, [&](uint16_t a, bool b){ExecASL(a,b);}}},
            {0x16, {ASL, ZeroPageX, 2, 6, 0, [&](uint16_t a, bool b){ExecASL(a,b);}}}, {0x0E, {ASL, Absolute, 3, 6, 0, [&](uint16_t a, bool b){ExecASL(a,b);}}},
            {0x1E, {ASL, AbsoluteX, 3, 6, 1, [&](uint16_t a, bool b){ExecASL(a,b);}}},

            // LSR
            {0x4A, {LSR, Accumulator, 1, 2, 0, [&](uint16_t a, bool b){ExecLSR(a,b);}}}, {0x46, {LSR, ZeroPage, 2, 5, 0, [&](uint16_t a, bool b){ExecLSR(a,b);}}},
            {0x56, {LSR, ZeroPageX, 2, 6, 0, [&](uint16_t a, bool b){ExecLSR(a,b);}}}, {0x4E, {LSR, Absolute, 3, 6, 0, [&](uint16_t a, bool b){ExecLSR(a,b);}}},
            {0x5E, {LSR, AbsoluteX, 3, 6, 1, [&](uint16_t a, bool b){ExecLSR(a,b);}}},

            // ROL
            {0x2A, {ROL, Accumulator, 1, 2, 0, [&](uint16_t a, bool b){ExecROL(a,b);}}}, {0x26, {ROL, ZeroPage, 2, 5, 0, [&](uint16_t a, bool b){ExecROL(a,b);}}},
            {0x36, {ROL, ZeroPageX, 2, 6, 0, [&](uint16_t a, bool b){ExecROL(a,b);}}}, {0x2E, {ROL, Absolute, 3, 6, 0, [&](uint16_t a, bool b){ExecROL(a,b);}}},
            {0x3E, {ROL, AbsoluteX, 3, 6, 1, [&](uint16_t a, bool b){ExecROL(a,b);}}},

            // ROR
            {0x6A, {ROR, Accumulator, 1, 2, 0, [&](uint16_t a, bool b){ExecROR(a,b);}}}, {0x66, {ROR, ZeroPage, 2, 5, 0, [&](uint16_t a, bool b){ExecROR(a,b);}}},
            {0x76, {ROR, ZeroPageX, 2, 6, 0, [&](uint16_t a, bool b){ExecROR(a,b);}}}, {0x6E, {ROR, Absolute, 3, 6, 0, [&](uint16_t a, bool b){ExecROR(a,b);}}},
            {0x7E, {ROR, AbsoluteX, 3, 6, 1, [&](uint16_t a, bool b){ExecROR(a,b);}}},

            // Relatives
            {0x90, {BCC, Relative, 2, 2, 1, [&](uint16_t a, bool b){ExecBCC(a,b);}}}, {0xB0, {BCS, Relative, 2, 2, 1, [&](uint16_t a, bool b){ExecBCS(a,b);}}},
            {0xF0, {BEQ, Relative, 2, 2, 1, [&](uint16_t a, bool b){ExecBEQ(a,b);}}}, {0xD0, {BNE, Relative, 2, 2, 1, [&](uint16_t a, bool b){ExecBNE(a,b);}}},
            {0x50, {BVC, Relative, 2, 2, 1, [&](uint16_t a, bool b){ExecBVC(a,b);}}}, {0x70, {BVS, Relative, 2, 2, 1, [&](uint16_t a, bool b){ExecBVS(a,b);}}},
            {0x10, {BPL, Relative, 2, 2, 1, [&](uint16_t a, bool b){ExecBPL(a,b);}}}, {0x30, {BMI, Relative, 2, 2, 1, [&](uint16_t a, bool b){ExecBMI(a,b);}}},

            // BIT
            {0x24, {BIT, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecBIT(a,b);}}}, {0x2C, {BIT, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecBIT(a,b);}}},

            // JMP
            {0x4C, {JMP, Absolute, 3, 3, 0, [&](uint16_t a, bool b){ExecJMP(a,b);}}}, {0x6C, {JMP, Indirect, 3, 5, 0, [&](uint16_t a, bool b){ExecJMP(a,b);}}},

            // JSR / RTS / BRK / RTI
            {0x20, {JSR, Absolute, 3, 6, 0, [&](uint16_t a, bool b){ExecJSR(a,b);}}}, {0x60, {RTS, Implied, 1, 6, 0, [&](uint16_t a, bool b){ExecRTS(a,b);}}},
            {0x00, {BRK, Implied, 1, 7, 0, [&](uint16_t a, bool b){ExecBRK(a,b);}}}, {0x40, {RTI, Implied, 1, 6, 0, [&](uint16_t a, bool b){ExecRTI(a,b);}}},

            // CMP
            {0xC9, {CMP, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecCMP(a,b);}}}, {0xC5, {CMP, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecCMP(a,b);}}},
            {0xD5, {CMP, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecCMP(a,b);}}}, {0xCD, {CMP, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecCMP(a,b);}}},
            {0xDD, {CMP, AbsoluteX, 3, 4, 1, [&](uint16_t a, bool b){ExecCMP(a,b);}}}, {0xD9, {CMP, AbsoluteY, 3, 4, 1, [&](uint16_t a, bool b){ExecCMP(a,b);}}},
            {0xC1, {CMP, IndexedIndirect, 2, 6, 0, [&](uint16_t a, bool b){ExecCMP(a,b);}}}, {0xD1, {CMP, IndirectIndexed, 2, 5, 1, [&](uint16_t a, bool b){ExecCMP(a,b);}}},

            // CPX
            {0xE0, {CPX, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecCPX(a,b);}}}, {0xE4, {CPX, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecCPX(a,b);}}},
            {0xEC, {CPX, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecCPX(a,b);}}},

            // CPY
            {0xC0, {CPY, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecCPY(a,b);}}}, {0xC4, {CPX, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecCPY(a,b);}}},
            {0xCC, {CPY, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecCPY(a,b);}}},

            // INC
            {0xE6, {INC, ZeroPage, 2, 5, 0, [&](uint16_t a, bool b){ExecINC(a,b);}}}, {0xF6, {INC, ZeroPageX, 2, 6, 0, [&](uint16_t a, bool b){ExecINC(a,b);}}},
            {0xEE, {INC, Absolute, 3, 6, 0, [&](uint16_t a, bool b){ExecINC(a,b);}}}, {0xFE, {INC, AbsoluteX, 3, 6, 1, [&](uint16_t a, bool b){ExecINC(a,b);}}},

            // DEC
            {0xC6, {DEC, ZeroPage, 2, 5, 0, [&](uint16_t a, bool b){ExecDEC(a,b);}}}, {0xD6, {DEC, ZeroPageX, 2, 6, 0, [&](uint16_t a, bool b){ExecDEC(a,b);}}},
            {0xCE, {DEC, Absolute, 3, 6, 0, [&](uint16_t a, bool b){ExecDEC(a,b);}}}, {0xDE, {DEC, AbsoluteX, 3, 6, 1, [&](uint16_t a, bool b){ExecDEC(a,b);}}},

            // DE{X,Y} / IN{X,Y}
            {0xE8, {INX, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecINX(a,b);}}}, {0xCA, {DEX, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecDEX(a,b);}}},
            {0xC8, {INY, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecINY(a,b);}}}, {0x88, {DEY, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecDEY(a,b);}}},

            // CL{C,I,D,V} / SE{C, I, D}
            {0x18, {CLC, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecCLC(a,b);}}}, {0x38, {SEC, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecSEC(a,b);}}},
            {0x58, {CLI, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecCLI(a,b);}}}, {0x78, {SEI, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecSEI(a,b);}}},
            {0xD8, {CLD, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecCLD(a,b);}}}, {0xF8, {SED, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecSED(a,b);}}},
            {0xB8, {CLV, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecCLV(a,b);}}},

            // LDA
            {0xA9, {LDA, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecLDA(a,b);}}}, {0xA5, {LDA, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecLDA(a,b);}}},
            {0xB5, {LDA, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecLDA(a,b);}}}, {0xAD, {LDA, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecLDA(a,b);}}},
            {0xBD, {LDA, AbsoluteX, 3, 4, 1, [&](uint16_t a, bool b){ExecLDA(a,b);}}}, {0xB9, {LDA, AbsoluteY, 3, 4, 1, [&](uint16_t a, bool b){ExecLDA(a,b);}}},
            {0xA9, {LDA, IndexedIndirect, 2, 6, 0, [&](uint16_t a, bool b){ExecLDA(a,b);}}}, {0xB1, {LDA, IndirectIndexed, 2, 5, 1, [&](uint16_t a, bool b){ExecLDA(a,b);}}},

            // LDX
            {0xA2, {LDX, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecLDX(a,b);}}}, {0xA6, {LDX, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecLDX(a,b);}}},
            {0xB6, {LDX, ZeroPageY, 2, 4, 0, [&](uint16_t a, bool b){ExecLDX(a,b);}}}, {0xAE, {LDX, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecLDX(a,b);}}},
            {0xBE, {LDX, AbsoluteY, 3, 4, 1, [&](uint16_t a, bool b){ExecLDX(a,b);}}},

            // LDY
            {0xA0, {LDY, Immediate, 2, 2, 0, [&](uint16_t a, bool b){ExecLDY(a,b);}}}, {0xA4, {LDY, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecLDY(a,b);}}},
            {0xB4, {LDY, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecLDY(a,b);}}}, {0xAC, {LDY, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecLDY(a,b);}}},
            {0xBC, {LDY, AbsoluteX, 3, 4, 1, [&](uint16_t a, bool b){ExecLDY(a,b);}}},

            // STA
            {0x85, {STA, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecSTA(a,b);}}}, {0x95, {LDA, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecSTA(a,b);}}},
            {0x8D, {STA, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecSTA(a,b);}}}, {0x9D, {LDA, AbsoluteX, 3, 4, 1, [&](uint16_t a, bool b){ExecSTA(a,b);}}},
            {0x99, {STA, AbsoluteY, 3, 4, 1, [&](uint16_t a, bool b){ExecSTA(a,b);}}}, {0x81, {LDA, IndexedIndirect, 2, 6,01, [&](uint16_t a, bool b){ExecSTA(a,b);}}},
            {0x91, {STA, IndirectIndexed, 2, 5, 1, [&](uint16_t a, bool b){ExecSTA(a,b);}}},

            // STX
            {0x86, {STX, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecSTX(a,b);}}}, {0x96, {STX, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecSTX(a,b);}}},
            {0x8E, {STX, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecSTX(a,b);}}},

            // STY
            {0x84, {STY, ZeroPage, 2, 3, 0, [&](uint16_t a, bool b){ExecSTY(a,b);}}}, {0x94, {STY, ZeroPageX, 2, 4, 0, [&](uint16_t a, bool b){ExecSTY(a,b);}}},
            {0x8C, {STY, Absolute, 3, 4, 0, [&](uint16_t a, bool b){ExecSTY(a,b);}}},

            // transfer related
            {0xAA, {TAX, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecTAX(a,b);}}}, {0x8A, {TXA, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecTXA(a,b);}}},
            {0xA8, {TAY, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecTAY(a,b);}}}, {0x98, {TYA, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecTYA(a,b);}}},
            {0x9A, {TXS, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecTXS(a,b);}}}, {0xBA, {TSX, Implied, 1, 2, 0, [&](uint16_t a, bool b){ExecTSX(a,b);}}},

            // push related
            {0x48, {PHA, Implied, 1, 3, 0, [&](uint16_t a, bool b){ExecPHA(a,b);}}},{0x68, {PLA, Implied, 1, 4, 0, [&](uint16_t a, bool b){ExecPLA(a,b);}}},
            {0x08, {PHP, Implied, 1, 3, 0, [&](uint16_t a, bool b){ExecPHP(a,b);}}}, {0x28, {PLP, Implied, 1, 4, 0, [&](uint16_t a, bool b){ExecPLP(a,b);}}},

            // NOP
            {0xEA, {NOP, Implied, 1, 2, 0, [&](uint16_t a, bool b){}}}
        };

        // debugging purpose
        idToInstructionName = {
            {ADC, "ADC"}, {AHX, "AHX"}, {ALR, "ALR"}, {ANC, "ANC"}, {AND, "AND"},
            {ARR, "ARR"}, {ASL, "ASL"}, {AXS, "AXS"}, {BCC, "BCC"}, {BCS, "BCS"},
            {BEQ, "BEQ"}, {BIT, "BIT"}, {BMI, "BMI"}, {BNE, "BNE"}, {BPL, "BPL"},
            {BRK, "BRK"}, {BVC, "BVC"}, {BVS, "BVS"}, {CLC, "CLC"}, {CLD, "CLD"},
            {CLI, "CLI"}, {CLV, "CLV"}, {CMP, "CMP"}, {CPX, "CPX"}, {CPY, "CPY"},
            {DCP, "DCP"}, {DEC, "DEC"}, {DEX, "DEX"}, {DEY, "DEY"}, {EOR, "EOR"},
            {INC, "INC"}, {INX, "INX"}, {INY, "INY"}, {ISC, "ISC"}, {JMP, "JMP"},
            {JSR, "JSR"}, {KIL, "KIL"}, {LAS, "LAS"}, {LAX, "LAX"}, {LDA, "LDA"},
            {LDX, "LDX"}, {LDY, "LDY"}, {LSR, "LSR"}, {NOP, "NOP"}, {ORA, "ORA"},
            {PHA, "PHA"}, {PHP, "PHP"}, {PLA, "PLA"}, {PLP, "PLP"}, {RLA, "RLA"},
            {ROL, "ROL"}, {ROR, "ROR"}, {RRA, "RRA"}, {RTI, "RTI"}, {RTS, "RTS"},
            {SAX, "SAX"}, {SBC, "SBC"}, {SEC, "SEC"}, {SED, "SED"}, {SEI, "SEI"},
            {SHX, "SHX"}, {SHY, "SHY"}, {SLO, "SLO"}, {SRE, "SRE"}, {STA, "STA"},
            {STX, "STX"}, {STY, "STY"}, {TAS, "TAS"}, {TAX, "TAX"}, {TAY, "TAY"},
            {TSX, "TSX"}, {TXA, "TXA"}, {TXS, "TXS"}, {TYA, "TYA"}, {XAA, "XAA"}
        };

        // setup memory interface
        mem = m;
        Reset();
    }
}

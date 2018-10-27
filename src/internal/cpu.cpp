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
        return (a&0xFF00) == (b&0xFF00);
    }

    // ADC instruction
    void Cpu::_adc(uint16_t address, bool is_accumulator){
        uint8_t a = A;
        uint8_t b = Memory->Read(address);
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

    void execADC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_adc(address, is_accumulator);
    }


    // AND instruction
    void Cpu::_and(uint16_t address, bool is_accumulator){
        A = A & Memory->Read(address);
        // set Z & N flags
        setZN(A);
    }

    void execAND(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_and(address, is_accumulator);
    }


    // ASL instruction
    void Cpu::_asl(uint16_t address, bool is_accumulator){
        if (is_accumulator) {
            C = (C >> 7) & uint8_t(1);
            A <<= 1;
            setZN(A);
        } else {
            uint8_t v = Memory->Read(address);
            C = (v >> 7) & uint8_t(1);
            v <<= 1;
            Memory->Write(address, v);
            setZN(v);
        }
    }

    void execASL(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_asl(address, is_accumulator);
    }


    // BCC instruction
    void Cpu::_bcc(uint16_t address, bool is_accumulator){
        if (C == 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        };
    };

    void execBCC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bcc(address, is_accumulator);
    }


    // BCS instruction
    void Cpu::_bcs(uint16_t address, bool is_accumulator){
        if (C != 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    void execBCS(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bcs(address, is_accumulator);
    }


    // BEQ instruction
    void Cpu::_beq(uint16_t address, bool is_accumulator){
        if (Z != 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    void execBEQ(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bcs(address, is_accumulator);
    }


    // BIT operation
    void Cpu::_bit(uint16_t address, bool is_accumulator){
        uint8_t v = Memory->Read(address);
        V = (v >> 6) & uint8_t(1);
        setZ(v & A);
        setN(v);
    };

    void execBIT(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bit(address, is_accumulator);
    }


    // BMI params
    void Cpu::_bmi(uint16_t address, bool is_accumulator){
        if (N != 0){
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    void execBMI(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bmi(address, is_accumulator);
    }


    // BNE operation
    void Cpu::_bne(uint16_t address, bool is_accumulator){
        if (Z == 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    void execBNE(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bne(address, is_accumulator);
    }


    // BQL operation
    void Cpu::_bql(uint16_t address, bool is_accumulator){
        if (N == 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    void execBQL(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bql(address, is_accumulator);
    }


    // BRK instruction
    void Cpu::_brk(uint16_t address, bool is_accumulator){
        push16(PC);
        _php(address, is_accumulator);
        _sei(address, is_accumulator);
        PC = read16(0xFFFE);
    };

    void execBRK(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_brk(address, is_accumulator);
    }


    // BVC instruction
    void Cpu::_bvc(uint16_t address, bool is_accumulator){
        if (V == 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    void execBVC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_brk(address, is_accumulator);
    }


    // BVS instruction
    void Cpu::_bvs(uint16_t address, bool is_accumulator){
        if (V != 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    void execBVS(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bvs(address, is_accumulator);
    }


    // CLC instruction
    void Cpu::_clc(uint16_t address, bool is_accumulator){
        C = 0;
    };

    void execCLC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_clc(address, is_accumulator);
    }


    // CLD instruction
    void Cpu::_cld(uint16_t address, bool is_accumulator){
        D = 0;
    };

    void execCLD(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_cld(address, is_accumulator);
    }


    // CLI instruction
    void Cpu::_cli(uint16_t address, bool is_accumulator){
        I = 0;
    };

    void execCLI(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_cli(address, is_accumulator);
    }


    // CLV instruction
    void Cpu::_clv(uint16_t address, bool is_accumulator){
        V = 0;
    };

    void execCLV(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_clv(address, is_accumulator);
    }


    // CMP instruction
    void Cpu::_cmp(uint16_t address, bool is_accumulator){
        uint8_t v = Memory->Read(address);
        compare(A, v);
    };

    void execCMP(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_cmp(address, is_accumulator);
    }


    // CPX instruction
    void Cpu::_cpx(uint16_t address, bool is_accumulator){
        uint8_t v = Memory->Read(address);
        _cmp(X, v);
    };

    void execCPX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_cpx(address, is_accumulator);
    }

    // CPY instruction
    void Cpu::_cpy(uint16_t address, bool is_accumulator){
        uint8_t v = Memory->Read(address);
        _cmp(Y, v);
    };

    void execCPY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_cpy(address, is_accumulator);
    }


    // DEC instruction
    void Cpu::_dec(uint16_t address, bool is_accumulator){
        uint8_t v = Memory->Read(address) - uint8_t(1);
        Memory->Write(address, v);
        setZN(v);
    };

    void execDEC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_dec(address, is_accumulator);
    }


    // DEX instruction
    void Cpu::_dex(uint16_t address, bool is_accumulator){
        --X;
        setZN(X);
    };

    void execDEX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_dex(address, is_accumulator);
    }


    // DEY instruction
    void Cpu::_dey(uint16_t address, bool is_accumulator){
        --Y;
        setZN(Y);
    };

    void execDEY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_dey(address, is_accumulator);
    }


    // EOR instruction
    void Cpu::_eor(uint16_t address, bool is_accumulator){
        A = A ^ Memory->Read(address);
        setZN(A);
    };

    void execEOR(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_eor(address, is_accumulator);
    }


    // INC instruction
    void Cpu::_inc(uint16_t address, bool is_accumulator){
        uint8_t v = Memory->Read(address) + uint8_t(1);
        Memory->Write(address, v);
        setZN(v);
    };

    void execINC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_inc(address, is_accumulator);
    }


    // INX instruction
    void Cpu::_inx(uint16_t address, bool is_accumulator){
        ++X;
        setZN(X);
    };

    void execINX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_inx(address, is_accumulator);
    }


    // INY instruction
    void Cpu::_iny(uint16_t address, bool is_accumulator){
        ++Y;
        setZN(Y);
    };

    void execINY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_iny(address, is_accumulator);
    }


    // JMP operation
    void Cpu::_jmp(uint16_t address, bool is_accumulator){
        PC = address;
    };

    void execJMP(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_jmp(address, is_accumulator);
    }


    // JSR operation
    void Cpu::_jsr(uint16_t address, bool is_accumulator){
        push16(PC - uint8_t(1));
        PC = address;
    };

    void execJSR(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_jsr(address, is_accumulator);
    }


    // LDA operation
    void Cpu::_lda(uint16_t address, bool is_accumulator){
        A = Memory->Read(address);
        setZN(A);
    };

    void execLDA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_lda(address, is_accumulator);
    }


    // LDX operation
    void Cpu::_ldx(uint16_t address, bool is_accumulator){
        X = Memory->Read(address);
        setZN(X);
    };

    void execLDX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_ldx(address, is_accumulator);
    }


    // LDY operation
    void Cpu::_ldy(uint16_t address, bool is_accumulator){
        Y = Memory->Read(address);
        setZN(Y);
    };

    void execLDY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_ldy(address, is_accumulator);
    }


    // LSR operation
    void Cpu::_lsr(uint16_t address, bool is_accumulator){
        if (is_accumulator) {
            C = A & uint8_t(1);
            A >>= 1;
            setZN(A);
        } else {
            uint8_t v = Memory->Read(address);
            C = v & uint8_t(1);
            v >>= 1;
            Memory->Write(address, v);
            setZN(v);
        }
    };

    void execLSR(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_lsr(address, is_accumulator);
    }


    // NOP operation
    void Cpu::_nop(uint16_t address, bool is_accumulator){};
    void execNOP(Cpu *cpu, uint16_t address, bool is_accumulator) {}


    // ORA operation
    void Cpu::_ora(uint16_t address, bool is_accumulator){
        A = A | Memory->Read(address);
        setZN(A);
    };

    void execORA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_ora(address, is_accumulator);
    }


    // PHA operation
    void Cpu::_pha(uint16_t address, bool is_accumulator){
        push(A);
    };

    void execPHA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_pha(address, is_accumulator);
    }


    // PHP operation
    void Cpu::_php(uint16_t address, bool is_accumulator){
        push(getFlag()|uint8_t(0x10));
    };

    void execPHP(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_php(address, is_accumulator);
    }


    // PLA opeartion
    void Cpu::_pla(uint16_t address, bool is_accumulator){
        A = pull();
        setZN(A);
    };

    void execPLA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_pla(address, is_accumulator);
    }


    // PLP operation
    void Cpu::_plp(uint16_t address, bool is_accumulator){
        setFlags((pull()&uint8_t(0xEF))| uint8_t(0x20));
    };

    void execPLP(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_plp(address, is_accumulator);
    }


    // ROL operation
    void Cpu::_rol(uint16_t address, bool is_accumulator){
        if (is_accumulator) {
            uint8_t prev_c = C;
            C = (A >> 7) & uint8_t(1);
            A = (A << 1) | prev_c;
            setZN(A);
        } else {
            uint8_t prev_c = C;
            uint8_t v = Memory->Read(address);
            C = (v >> 7) & uint8_t(1);
            v = (v << 1) | prev_c;
            Memory->Write(address, v);
            setZN(v);
        }
    };

    void execROL(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_rol(address, is_accumulator);
    }


    // ROR operation
    void Cpu::_ror(uint16_t address, bool is_accumulator){
        if (is_accumulator) {
            uint8_t prev_c = C;
            C = A & uint8_t(1);
            A = (A >> 1) | (prev_c << 7);
            setZN(A);
        } else {
            uint8_t prev_c = C;
            uint8_t v = Memory->Read(address);
            C = v & uint8_t(1);
            v = (v >> 1) | (prev_c << 7);
            Memory->Write(address, v);
            setZN(v);
        }
    };

    void execROR(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_ror(address, is_accumulator);
    }


    // RTI operation
    void Cpu::_rti(uint16_t address, bool is_accumulator){
        setFlags((pull()&uint8_t(0xEF))| uint8_t(0x20));
        PC = pull16();
    };

    void execRTI(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_rti(address, is_accumulator);
    }


    // RTS operation
    void Cpu::_rts(uint16_t address, bool is_accumulator){
        PC = pull16() + uint16_t(1);
    };

    void execRTS(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_rts(address, is_accumulator);
    }


    // SBC operation
    void Cpu::_sbc(uint16_t address, bool is_accumulator){
        uint8_t prev_a = A;
        uint8_t b = Memory->Read(address);
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

    void execSBC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sbc(address, is_accumulator);
    }

    // SEC operation
    void Cpu::_sec(uint16_t address, bool is_accumulator){
        C = 1;
    };

    void execSEC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sec(address, is_accumulator);
    }


    // SED operation
    void Cpu::_sed(uint16_t address, bool is_accumulator){
        D = 1;
    };

    void execSED(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sed(address, is_accumulator);
    }


    // SEI operation
    void Cpu::_sei(uint16_t address, bool is_accumulator){
        I = 1;
    };

    void execSEI(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sei(address, is_accumulator);
    }


    // STA operation
    void Cpu::_sta(uint16_t address, bool is_accumulator){
        Memory->Write(address, A);
    };

    void execSEA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sta(address, is_accumulator);
    }


    // STX operation
    void Cpu::_stx(uint16_t address, bool is_accumulator){
        Memory->Write(address, X);
    };

    void execSEX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_stx(address, is_accumulator);
    }


    // STY operation
    void Cpu::_sty(uint16_t address, bool is_accumulator){
        Memory->Write(address, Y);
    };

    void execSEY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sty(address, is_accumulator);
    }


    // TAX operation
    void Cpu::_tax(uint16_t address, bool is_accumulator){
        X = A;
        setZN(X);
    };

    void execTAX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_tax(address, is_accumulator);
    }


    // TAY operation
    void Cpu::_tay(uint16_t address, bool is_accumulator){
        Y = A;
        setZN(Y);
    };

    void execTAY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_tay(address, is_accumulator);
    }


    // TSX operation
    void Cpu::_tsx(uint16_t address, bool is_accumulator){
        X = SP;
        setZN(X);
    };

    void execTSX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_tsx(address, is_accumulator);
    }


    // TXA operation
    void Cpu::_txa(uint16_t address, bool is_accumulator){
        A = X;
        setZN(A);
    };

    void execTXA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_txa(address, is_accumulator);
    }


    // TXS operation
    void Cpu::_txs(uint16_t address, bool is_accumulator){
        SP = X;
    };

    void execTXS(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_txs(address, is_accumulator);
    }

    // TYA operation
    void Cpu::_tya(uint16_t address, bool is_accumulator){
        A = Y;
        setZN(A);
    };

    void execTYA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_tya(address, is_accumulator);
    }

    // represents illegal operation ... DO NOTHING
    void Cpu::illegal(uint16_t address, bool is_accumulator){};

    uint16_t Cpu::read16(uint16_t address) {
        uint16_t lo = Memory->Read(address);
        uint16_t hi = Memory->Read(address + uint16_t(1)) <<8;
        return hi | lo;
    }

    uint16_t Cpu::read16Bug(uint16_t address) {
        uint16_t lo = address;
        uint16_t hi = (address&uint16_t(0xFF00)) | uint16_t(address+1) <<8;
        return uint16_t(hi)<<8 | uint16_t(lo);
    }

    void Cpu::push(uint8_t v) {
        Memory->Write(uint16_t(0x100)|uint16_t(SP), v);
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
        C = (flag >> 0) & uint8_t(1);
        Z = (flag >> 1) & uint8_t(1);
        I = (flag >> 2) & uint8_t(1);
        D = (flag >> 3) & uint8_t(1);
        B = (flag >> 4) & uint8_t(1);
        U = (flag >> 5) & uint8_t(1);
        V = (flag >> 6) & uint8_t(1);
        N = (flag >> 7) & uint8_t(1);
    }


    uint8_t Cpu::getFlag() {
        uint8_t flag = 0;
        flag |= C << 0;
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
        return Memory->Read(uint16_t(0x100)|uint16_t(SP));
    }

    uint16_t Cpu::pull16() {
        return uint16_t(pull()) << 8 | uint16_t(pull());
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
        _php(0, false);
        PC = read16(0xFFFE);
        I = 1;
        Cycles += 7;
    }

    void Cpu::nmi() {
        push16(PC);
        _php(0, false);
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
        uint8_t op = Memory->Read(PC);
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
                address = read16Bug(Memory->Read(PC + uint16_t(1)) + uint16_t(X));
                break;
            }
            case Indirect: {
                address = read16Bug(read16(PC + uint16_t(1)));
                break;
            }
            case IndirectIndexed: {
                address = read16Bug(Memory->Read(PC + uint16_t(1))) + uint16_t(Y);
                page_crossed = isPageCrossed(address - uint16_t(Y), address);
                break;
            }
            case Relative: {
                uint16_t offset = Memory->Read(PC + uint16_t(1));
                if (offset < 0x80) {
                    address = PC + uint16_t(2) + offset;
                } else {
                    address = PC + uint16_t(2) + offset - uint16_t(0x100);
                }
                break;
            }
            case ZeroPage: {
                address = Memory->Read(PC + uint16_t(1));
                break;
            }
            case ZeroPageX: {
                address = uint16_t((Memory->Read(PC + uint16_t(1)) + X) & 0xff);
                break;
            }
            case ZeroPageY: {
                address = uint16_t((Memory->Read(PC + uint16_t(1)) + Y) & 0xff);
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

        inst.executor(this, address, inst.AddressingMode == Accumulator);

        if (IsDebugMode) {
            cout << "[Instruction]:" << idToInstructionName.find(inst.ID)->second;
            cout << "[address]: " << address;
            cout << "[addressing mode]: " << inst.AddressingMode << "\n";
        }

        return Cycles - prev_cycles;
    }

    void Setup() {
        // setup instruction table
        nestake::InstructionTable = {
                // ADC instructions
                {0x69, {ADC, Immediate, 2, 2, 0, execADC}}, {0x65, {ADC, ZeroPage, 2, 3, 0, execADC}},
                {0x75, {ADC, ZeroPageX, 2, 4, 0, execADC}}, {0x6D, {ADC, Absolute, 3, 4, 0, execADC}},
                {0x7D, {ADC, AbsoluteX, 3, 4, 1, execADC}}, {0x79, {ADC, AbsoluteY, 3, 4, 1, execADC}},
                {0x79, {ADC, IndexedIndirect, 2, 6, 0, execADC}}, {0x79, {ADC, IndirectIndexed, 2, 5, 1, execADC}},


                // TODO add more instructions
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
    }
}

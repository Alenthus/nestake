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
    void Cpu::_adc(uint16_t address, bool){
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

    void execADC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_adc(address, is_accumulator);
    }


    // AND instruction
    void Cpu::_and(uint16_t address, bool){
        A = A & mem->Read(address);
        // set Z & N flags
        setZN(A);
    }

    void execAND(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_and(address, is_accumulator);
    }


    // ASL instruction
    void Cpu::_asl(uint16_t address, bool is_accumulator){
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

    void execASL(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_asl(address, is_accumulator);
    }


    // BCC instruction
    void Cpu::_bcc(uint16_t address, bool){
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
    void Cpu::_bcs(uint16_t address, bool){
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
    void Cpu::_beq(uint16_t address, bool){
        if (Z != 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    void execBEQ(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_beq(address, is_accumulator);
    }


    // BIT operation
    void Cpu::_bit(uint16_t address, bool){
        uint8_t v = mem->Read(address);
        setZ(v & A);
        V = (v >> 6) & uint8_t(1);
        setN((v >> 7) & uint8_t(1));
    };

    void execBIT(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bit(address, is_accumulator);
    }


    // BMI params
    void Cpu::_bmi(uint16_t address, bool){
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
    void Cpu::_bne(uint16_t address, bool){
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


    // BPL operation
    void Cpu::_bpl(uint16_t address, bool){
        if (N == 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    void execBPL(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bpl(address, is_accumulator);
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
    void Cpu::_bvc(uint16_t address, bool){
        if (V == 0) {
            if (isPageCrossed(PC, address)) {
                ++Cycles;
            }
            ++Cycles;
            PC = address;
        }
    };

    void execBVC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_bvc(address, is_accumulator);
    }


    // BVS instruction
    void Cpu::_bvs(uint16_t address, bool){
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
    void Cpu::_clc(uint16_t, bool){
        C = 0;
    };

    void execCLC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_clc(address, is_accumulator);
    }


    // CLD instruction
    void Cpu::_cld(uint16_t, bool){
        D = 0;
    };

    void execCLD(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_cld(address, is_accumulator);
    }


    // CLI instruction
    void Cpu::_cli(uint16_t, bool){
        I = 0;
    };

    void execCLI(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_cli(address, is_accumulator);
    }


    // CLV instruction
    void Cpu::_clv(uint16_t, bool){
        V = 0;
    };

    void execCLV(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_clv(address, is_accumulator);
    }


    // CMP instruction
    void Cpu::_cmp(uint16_t address, bool){
        uint8_t v = mem->Read(address);
        compare(A, v);
    };

    void execCMP(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_cmp(address, is_accumulator);
    }


    // CPX instruction
    void Cpu::_cpx(uint16_t address, bool){
        uint8_t v = mem->Read(address);
        _cmp(X, v);
    };

    void execCPX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_cpx(address, is_accumulator);
    }

    // CPY instruction
    void Cpu::_cpy(uint16_t address, bool){
        uint8_t v = mem->Read(address);
        _cmp(Y, v);
    };

    void execCPY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_cpy(address, is_accumulator);
    }


    // DEC instruction
    void Cpu::_dec(uint16_t address, bool){
        uint8_t v = mem->Read(address) - uint8_t(1);
        mem->Write(address, v);
        setZN(v);
    };

    void execDEC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_dec(address, is_accumulator);
    }


    // DEX instruction
    void Cpu::_dex(uint16_t, bool){
        --X;
        setZN(X);
    };

    void execDEX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_dex(address, is_accumulator);
    }


    // DEY instruction
    void Cpu::_dey(uint16_t, bool){
        --Y;
        setZN(Y);
    };

    void execDEY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_dey(address, is_accumulator);
    }


    // EOR instruction
    void Cpu::_eor(uint16_t address, bool){
        A = A ^ mem->Read(address);
        setZN(A);
    };

    void execEOR(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_eor(address, is_accumulator);
    }


    // INC instruction
    void Cpu::_inc(uint16_t address, bool){
        uint8_t v = mem->Read(address) + uint8_t(1);
        mem->Write(address, v);
        setZN(v);
    };

    void execINC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_inc(address, is_accumulator);
    }


    // INX instruction
    void Cpu::_inx(uint16_t, bool){
        ++X;
        setZN(X);
    };

    void execINX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_inx(address, is_accumulator);
    }


    // INY instruction
    void Cpu::_iny(uint16_t, bool){
        ++Y;
        setZN(Y);
    };

    void execINY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_iny(address, is_accumulator);
    }


    // JMP operation
    void Cpu::_jmp(uint16_t address, bool){
        PC = address;
    };

    void execJMP(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_jmp(address, is_accumulator);
    }


    // JSR operation
    void Cpu::_jsr(uint16_t address, bool){
        push16(PC - uint8_t(1));
        PC = address;
    };

    void execJSR(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_jsr(address, is_accumulator);
    }


    // LDA operation
    void Cpu::_lda(uint16_t address, bool){
        A = mem->Read(address);
        setZN(A);
    };

    void execLDA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_lda(address, is_accumulator);
    }


    // LDX operation
    void Cpu::_ldx(uint16_t address, bool){
        X = mem->Read(address);
        setZN(X);
    };

    void execLDX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_ldx(address, is_accumulator);
    }


    // LDY operation
    void Cpu::_ldy(uint16_t address, bool){
        Y = mem->Read(address);
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
            uint8_t v = mem->Read(address);
            C = v & uint8_t(1);
            v >>= 1;
            mem->Write(address, v);
            setZN(v);
        }
    };

    void execLSR(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_lsr(address, is_accumulator);
    }


    // NOP operation
    void execNOP(Cpu*, uint16_t, bool) {}


    // ORA operation
    void Cpu::_ora(uint16_t address, bool){
        A = A | mem->Read(address);
        setZN(A);
    };

    void execORA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_ora(address, is_accumulator);
    }


    // PHA operation
    void Cpu::_pha(uint16_t, bool){
        push(A);
    };

    void execPHA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_pha(address, is_accumulator);
    }


    // PHP operation
    void Cpu::_php(uint16_t, bool){
        push(getFlag()|uint8_t(0x10));
    };

    void execPHP(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_php(address, is_accumulator);
    }


    // PLA operation
    void Cpu::_pla(uint16_t, bool){
        A = pull();
        setZN(A);
    };

    void execPLA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_pla(address, is_accumulator);
    }


    // PLP operation
    void Cpu::_plp(uint16_t, bool){
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
            uint8_t v = mem->Read(address);
            C = (v >> 7) & uint8_t(1);
            v = (v << 1) | prev_c;
            mem->Write(address, v);
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
            uint8_t v = mem->Read(address);
            C = v & uint8_t(1);
            v = (v >> 1) | (prev_c << 7);
            mem->Write(address, v);
            setZN(v);
        }
    };

    void execROR(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_ror(address, is_accumulator);
    }


    // RTI operation
    void Cpu::_rti(uint16_t, bool){
        setFlags((pull()&uint8_t(0xEF))| uint8_t(0x20));
        PC = pull16();
    };

    void execRTI(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_rti(address, is_accumulator);
    }


    // RTS operation
    void Cpu::_rts(uint16_t, bool){
        PC = pull16() + uint16_t(1);
    };

    void execRTS(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_rts(address, is_accumulator);
    }


    // SBC operation
    void Cpu::_sbc(uint16_t address, bool){
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

    void execSBC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sbc(address, is_accumulator);
    }

    // SEC operation
    void Cpu::_sec(uint16_t, bool){
        C = 1;
    };

    void execSEC(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sec(address, is_accumulator);
    }


    // SED operation
    void Cpu::_sed(uint16_t, bool){
        D = 1;
    };

    void execSED(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sed(address, is_accumulator);
    }


    // SEI operation
    void Cpu::_sei(uint16_t, bool){
        I = 1;
    };

    void execSEI(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sei(address, is_accumulator);
    }


    // STA operation
    void Cpu::_sta(uint16_t address, bool){
        mem->Write(address, A);
    };

    void execSTA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sta(address, is_accumulator);
    }


    // STX operation
    void Cpu::_stx(uint16_t address, bool){
        mem->Write(address, X);
    };

    void execSTX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_stx(address, is_accumulator);
    }


    // STY operation
    void Cpu::_sty(uint16_t address, bool){
        mem->Write(address, Y);
    };

    void execSTY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_sty(address, is_accumulator);
    }


    // TAX operation
    void Cpu::_tax(uint16_t, bool){
        X = A;
        setZN(X);
    };

    void execTAX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_tax(address, is_accumulator);
    }


    // TAY operation
    void Cpu::_tay(uint16_t, bool){
        Y = A;
        setZN(Y);
    };

    void execTAY(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_tay(address, is_accumulator);
    }


    // TSX operation
    void Cpu::_tsx(uint16_t, bool){
        X = SP;
        setZN(X);
    };

    void execTSX(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_tsx(address, is_accumulator);
    }


    // TXA operation
    void Cpu::_txa(uint16_t, bool){
        A = X;
        setZN(A);
    };

    void execTXA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_txa(address, is_accumulator);
    }


    // TXS operation
    void Cpu::_txs(uint16_t, bool){
        SP = X;
    };

    void execTXS(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_txs(address, is_accumulator);
    }


    // TYA operation
    void Cpu::_tya(uint16_t, bool){
        A = Y;
        setZN(A);
    };

    void execTYA(Cpu *cpu, uint16_t address, bool is_accumulator) {
        cpu->_tya(address, is_accumulator);
    }


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

        inst.executor(this, address, inst.AddressingMode == Accumulator);

        if (IsDebugMode) {
            cout << "[Instruction]:" << idToInstructionName.find(inst.ID)->second;
            cout << "[address]: " << address;
            cout << "[addressing mode]: " << inst.AddressingMode << "\n";
        }

        return Cycles - prev_cycles;
    }

    Cpu::Cpu(Memory* m) {
        // setup instruction table
        // ref: http://pgate1.at-ninja.jp/NES_on_FPGA/nes_cpu.htm#instruction
        InstructionTable = {
            // ADC
            {0x69, {ADC, Immediate, 2, 2, 0, execADC}}, {0x65, {ADC, ZeroPage, 2, 3, 0, execADC}},
            {0x75, {ADC, ZeroPageX, 2, 4, 0, execADC}}, {0x6D, {ADC, Absolute, 3, 4, 0, execADC}},
            {0x7D, {ADC, AbsoluteX, 3, 4, 1, execADC}}, {0x79, {ADC, AbsoluteY, 3, 4, 1, execADC}},
            {0x79, {ADC, IndexedIndirect, 2, 6, 0, execADC}}, {0x71, {ADC, IndirectIndexed, 2, 5, 1, execADC}},

            // SBC
            {0xE9, {SBC, Immediate, 2, 2, 0, execSBC}}, {0xE5, {SBC, ZeroPage, 2, 3, 0, execSBC}},
            {0xF5, {SBC, ZeroPageX, 2, 4, 0, execSBC}}, {0xED, {SBC, Absolute, 3, 4, 0, execSBC}},
            {0xFD, {SBC, AbsoluteX, 3, 4, 1, execSBC}}, {0xF9, {SBC, AbsoluteY, 3, 4, 1, execSBC}},
            {0xE1, {SBC, IndexedIndirect, 2, 6, 0, execSBC}}, {0xF1, {SBC, IndirectIndexed, 2, 5, 1, execSBC}},

            // AND
            {0x29, {AND, Immediate, 2, 2, 0, execAND}}, {0x25, {AND, ZeroPage, 2, 3, 0, execAND}},
            {0x35, {AND, ZeroPageX, 2, 4, 0, execAND}}, {0x2D, {AND, Absolute, 3, 4, 0, execAND}},
            {0x3D, {AND, AbsoluteX, 3, 4, 1, execAND}}, {0x39, {AND, AbsoluteY, 3, 4, 1, execAND}},
            {0x21, {AND, IndexedIndirect, 2, 6, 0, execAND}}, {0x31, {AND, IndirectIndexed, 2, 5, 1, execAND}},

            // ORA
            {0x09, {ORA, Immediate, 2, 2, 0, execORA}}, {0x05, {ORA, ZeroPage, 2, 3, 0, execORA}},
            {0x15, {ORA, ZeroPageX, 2, 4, 0, execORA}}, {0x0D, {ORA, Absolute, 3, 4, 0, execORA}},
            {0x1D, {ORA, AbsoluteX, 3, 4, 1, execORA}}, {0x19, {ORA, AbsoluteY, 3, 4, 1, execORA}},
            {0x01, {ORA, IndexedIndirect, 2, 6, 0, execORA}}, {0x11, {ORA, IndirectIndexed, 2, 5, 1, execORA}},

            // EOR
            {0x49, {EOR, Immediate, 2, 2, 0, execEOR}}, {0x45, {EOR, ZeroPage, 2, 3, 0, execEOR}},
            {0x55, {EOR, ZeroPageX, 2, 4, 0, execEOR}}, {0x4D, {EOR, Absolute, 3, 4, 0, execEOR}},
            {0x5D, {EOR, AbsoluteX, 3, 4, 1, execEOR}}, {0x59, {EOR, AbsoluteY, 3, 4, 1, execEOR}},
            {0x41, {EOR, IndexedIndirect, 2, 6, 0, execEOR}}, {0x51, {EOR, IndirectIndexed, 2, 5, 1, execEOR}},

            // ASL
            {0x0A, {ASL, Accumulator, 1, 2, 0, execASL}}, {0x06, {ASL, ZeroPage, 2, 5, 0, execASL}},
            {0x16, {ASL, ZeroPageX, 2, 6, 0, execASL}}, {0x0E, {ASL, Absolute, 3, 6, 0, execASL}},
            {0x1E, {ASL, AbsoluteX, 3, 6, 1, execASL}},

            // LSR
            {0x4A, {LSR, Accumulator, 1, 2, 0, execLSR}}, {0x46, {LSR, ZeroPage, 2, 5, 0, execLSR}},
            {0x56, {LSR, ZeroPageX, 2, 6, 0, execLSR}}, {0x4E, {LSR, Absolute, 3, 6, 0, execLSR}},
            {0x5E, {LSR, AbsoluteX, 3, 6, 1, execLSR}},

            // ROL
            {0x2A, {ROL, Accumulator, 1, 2, 0, execROL}}, {0x26, {ROL, ZeroPage, 2, 5, 0, execROL}},
            {0x36, {ROL, ZeroPageX, 2, 6, 0, execROL}}, {0x2E, {ROL, Absolute, 3, 6, 0, execROL}},
            {0x3E, {ROL, AbsoluteX, 3, 6, 1, execROL}},

            // ROR
            {0x6A, {ROR, Accumulator, 1, 2, 0, execROR}}, {0x66, {ROR, ZeroPage, 2, 5, 0, execROR}},
            {0x76, {ROR, ZeroPageX, 2, 6, 0, execROR}}, {0x6E, {ROR, Absolute, 3, 6, 0, execROR}},
            {0x7E, {ROR, AbsoluteX, 3, 6, 1, execROR}},

            // Relatives
            {0x90, {BCC, Relative, 2, 2, 1, execBCC}}, {0xB0, {BCS, Relative, 2, 2, 1, execBCS}},
            {0xF0, {BEQ, Relative, 2, 2, 1, execBEQ}}, {0xD0, {BNE, Relative, 2, 2, 1, execBNE}},
            {0x50, {BVC, Relative, 2, 2, 1, execBVC}}, {0x70, {BVS, Relative, 2, 2, 1, execBVS}},
            {0x10, {BPL, Relative, 2, 2, 1, execBPL}}, {0x30, {BMI, Relative, 2, 2, 1, execBMI}},

            // BIT
            {0x24, {BIT, ZeroPage, 2, 3, 0, execBIT}}, {0x2C, {BIT, Absolute, 3, 4, 0, execBIT}},

            // JMP
            {0x4C, {JMP, Absolute, 3, 3, 0, execJMP}}, {0x6C, {JMP, Indirect, 3, 5, 0, execJMP}},

            // JSR / RTS / BRK / RTI
            {0x20, {JSR, Absolute, 3, 6, 0, execJSR}}, {0x60, {RTS, Implied, 1, 6, 0, execRTS}},
            {0x00, {BRK, Implied, 1, 7, 0, execBRK}}, {0x40, {RTI, Implied, 1, 6, 0, execRTI}},

            // CMP
            {0xC9, {CMP, Immediate, 2, 2, 0, execCMP}}, {0xC5, {CMP, ZeroPage, 2, 3, 0, execCMP}},
            {0xD5, {CMP, ZeroPageX, 2, 4, 0, execCMP}}, {0xCD, {CMP, Absolute, 3, 4, 0, execCMP}},
            {0xDD, {CMP, AbsoluteX, 3, 4, 1, execCMP}}, {0xD9, {CMP, AbsoluteY, 3, 4, 1, execCMP}},
            {0xC1, {CMP, IndexedIndirect, 2, 6, 0, execCMP}}, {0xD1, {CMP, IndirectIndexed, 2, 5, 1, execCMP}},

            // CPX
            {0xE0, {CPX, Immediate, 2, 2, 0, execCPX}}, {0xE4, {CPX, ZeroPage, 2, 3, 0, execCPX}},
            {0xEC, {CPX, Absolute, 3, 4, 0, execCPX}},

            // CPY
            {0xC0, {CPY, Immediate, 2, 2, 0, execCPY}}, {0xC4, {CPX, ZeroPage, 2, 3, 0, execCPY}},
            {0xCC, {CPY, Absolute, 3, 4, 0, execCPY}},

            // INC
            {0xE6, {INC, ZeroPage, 2, 5, 0, execINC}}, {0xF6, {INC, ZeroPageX, 2, 6, 0, execINC}},
            {0xEE, {INC, Absolute, 3, 6, 0, execINC}}, {0xFE, {INC, AbsoluteX, 3, 6, 1, execINC}},

            // DEC
            {0xC6, {DEC, ZeroPage, 2, 5, 0, execDEC}}, {0xD6, {DEC, ZeroPageX, 2, 6, 0, execDEC}},
            {0xCE, {DEC, Absolute, 3, 6, 0, execDEC}}, {0xDE, {DEC, AbsoluteX, 3, 6, 1, execDEC}},

            // DE{X,Y} / IN{X,Y}
            {0xE8, {INX, Implied, 1, 2, 0, execINX}}, {0xCA, {DEX, Implied, 1, 2, 0, execDEX}},
            {0xC8, {INY, Implied, 1, 2, 0, execINY}}, {0x88, {DEY, Implied, 1, 2, 0, execDEY}},

            // CL{C,I,D,V} / SE{C, I, D}
            {0x18, {CLC, Implied, 1, 2, 0, execCLC}}, {0x38, {SEC, Implied, 1, 2, 0, execSEC}},
            {0x58, {CLI, Implied, 1, 2, 0, execCLI}}, {0x78, {SEI, Implied, 1, 2, 0, execSEI}},
            {0xD8, {CLD, Implied, 1, 2, 0, execCLD}}, {0xF8, {SED, Implied, 1, 2, 0, execSED}},
            {0xB8, {CLV, Implied, 1, 2, 0, execCLV}},

            // LDA
            {0xA9, {LDA, Immediate, 2, 2, 0, execLDA}}, {0xA5, {LDA, ZeroPage, 2, 3, 0, execLDA}},
            {0xB5, {LDA, ZeroPageX, 2, 4, 0, execLDA}}, {0xAD, {LDA, Absolute, 3, 4, 0, execLDA}},
            {0xBD, {LDA, AbsoluteX, 3, 4, 1, execLDA}}, {0xB9, {LDA, AbsoluteY, 3, 4, 1, execLDA}},
            {0xA9, {LDA, IndexedIndirect, 2, 6, 0, execLDA}}, {0xB1, {LDA, IndirectIndexed, 2, 5, 1, execLDA}},

            // LDX
            {0xA2, {LDX, Immediate, 2, 2, 0, execLDX}}, {0xA6, {LDX, ZeroPage, 2, 3, 0, execLDX}},
            {0xB6, {LDX, ZeroPageY, 2, 4, 0, execLDX}}, {0xAE, {LDX, Absolute, 3, 4, 0, execLDX}},
            {0xBE, {LDX, AbsoluteY, 3, 4, 1, execLDX}},

            // LDY
            {0xA0, {LDY, Immediate, 2, 2, 0, execLDY}}, {0xA4, {LDY, ZeroPage, 2, 3, 0, execLDY}},
            {0xB4, {LDY, ZeroPageX, 2, 4, 0, execLDY}}, {0xAC, {LDY, Absolute, 3, 4, 0, execLDY}},
            {0xBC, {LDY, AbsoluteX, 3, 4, 1, execLDY}},

            // STA
            {0x85, {STA, ZeroPage, 2, 3, 0, execSTA}}, {0x95, {LDA, ZeroPageX, 2, 4, 0, execSTA}},
            {0x8D, {STA, Absolute, 3, 4, 0, execSTA}}, {0x9D, {LDA, AbsoluteX, 3, 4, 1, execSTA}},
            {0x99, {STA, AbsoluteY, 3, 4, 1, execSTA}}, {0x81, {LDA, IndexedIndirect, 2, 6,01, execSTA}},
            {0x91, {STA, IndirectIndexed, 2, 5, 1, execSTA}},

            // STX
            {0x86, {STX, ZeroPage, 2, 3, 0, execSTX}}, {0x96, {STX, ZeroPageX, 2, 4, 0, execSTX}},
            {0x8E, {STX, Absolute, 3, 4, 0, execSTX}},

            // STY
            {0x84, {STY, ZeroPage, 2, 3, 0, execSTY}}, {0x94, {STY, ZeroPageX, 2, 4, 0, execSTY}},
            {0x8C, {STY, Absolute, 3, 4, 0, execSTY}},

            // transfer related
            {0xAA, {TAX, Implied, 1, 2, 0, execTAX}}, {0x8A, {TXA, Implied, 1, 2, 0, execTXA}},
            {0xA8, {TAY, Implied, 1, 2, 0, execTAY}}, {0x98, {TYA, Implied, 1, 2, 0, execTYA}},
            {0x9A, {TXS, Implied, 1, 2, 0, execTXS}}, {0xBA, {TSX, Implied, 1, 2, 0, execTSX}},

            // push related
            {0x48, {PHA, Implied, 1, 3, 0, execPHA}}, {0x68, {PLA, Implied, 1, 4, 0, execPLA}},
            {0x08, {PHP, Implied, 1, 3, 0, execPHP}}, {0x28, {PLP, Implied, 1, 4, 0, execPLP}},

            // NOP
            {0xEA, {NOP, Implied, 1, 2, 0, execNOP}}
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

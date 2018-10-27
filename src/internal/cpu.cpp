#include <string>
#include <iostream>
#include "cpu.hpp"
#include "memory.cpp"

// Addressing mode struct for expressing each addressing mode as int

using std::array;
using std::cout;
using std::map;
using std::string;

namespace nestake {

    // instructions's identifier supposed to be used in debugging
    enum InstructionID {
        ADC = 1, AHX, ALR, ANC, AND, ARR, ASL, AXS, BCC, BCS, BEQ, BIT, BMI,
        BNE, BPL, BRK, BVC, BVS, CLC, CLD, CLI, CLV, CMP, CPX, CPY, DCP, DEC,
        DEX, DEY, EOR, INC, INX, INY, ISC, JMP, JSR, KIL, LAS, LAX, LDA, LDX,
        LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, RLA, ROL, ROR, RRA, RTI, RTS,
        SAX, SBC, SEC, SED, SEI, SHX, SHY, SLO, SRE, STA, STX, STY, TAS, TAX,
        TAY, TSX, TXA, TXS, TYA, XAA,
    };

    map<int, string> idToInstructionName = {
        {ADC, "ADC"}, {AHX, "AHX"}, {ALR, "ALR"}, {ANC, "ANC"}, {AND, "AND"}, {ARR, "ARR"}, {ASL, "ASL"}, {AXS, "AXS"}, {BCC, "BCC"}, {BCS, "BCS"}, {BEQ, "BEQ"}, {BIT, "BIT"}, {BMI, "BMI"}, {BNE, "BNE"}, {BPL, "BPL"}, {BRK, "BRK"},
        {BVC, "BVC"}, {BVS, "BVS"}, {CLC, "CLC"}, {CLD, "CLD"}, {CLI, "CLI"}, {CLV, "CLV"}, {CMP, "CMP"}, {CPX, "CPX"}, {CPY, "CPY"}, {DCP, "DCP"}, {DEC, "DEC"}, {DEX, "DEX"}, {DEY, "DEY"}, {EOR, "EOR"}, {INC, "INC"}, {INX, "INX"},
        {INY, "INY"}, {ISC, "ISC"}, {JMP, "JMP"}, {JSR, "JSR"}, {KIL, "KIL"}, {LAS, "LAS"}, {LAX, "LAX"}, {LDA, "LDA"}, {LDX, "LDX"}, {LDY, "LDY"}, {LSR, "LSR"}, {NOP, "NOP"}, {ORA, "ORA"}, {PHA, "PHA"}, {PHP, "PHP"}, {PLA, "PLA"},
        {PLP, "PLP"}, {RLA, "RLA"}, {ROL, "ROL"}, {ROR, "ROR"}, {RRA, "RRA"}, {RTI, "RTI"}, {RTS, "RTS"}, {SAX, "SAX"}, {SBC, "SBC"}, {SEC, "SEC"}, {SED, "SED"}, {SEI, "SEI"}, {SHX, "SHX"}, {SHY, "SHY"}, {SLO, "SLO"}, {SRE, "SRE"},
        {STA, "STA"}, {STX, "STX"}, {STY, "STY"}, {TAS, "TAS"}, {TAX, "TAX"}, {TAY, "TAY"}, {TSX, "TSX"}, {TXA, "TXA"}, {TXS, "TXS"}, {TYA, "TYA"}, {XAA, "XAA"}
    };

    void _adc(Cpu* cpu, uint16_t address){
        uint8_t a = cpu->A;
        uint8_t b = cpu->Memory->Read(address);
        cpu->A = a + b + cpu->C;

        // set Z & N flags
        cpu->setZN(cpu->A);

        // set carry flag
        if ((int(a) + int(b) + int(cpu->C)) > 0xFF) {
            cpu->C = 1;
        } else {
            cpu->C = 0;
        }

        // set overflow flag
        if ((((a^b)&0x80) == 0) && (a^cpu->A != 0)){
            cpu->V = 1;
        } else {
            cpu->V = 0;
        }
    }

    void _and(Cpu* cpu, uint16_t address, bool is_accumulator){
        cpu->A = cpu->A & cpu->Memory->Read(address);
        // set Z & N flags
        cpu->setZN(cpu->A);
    }

    void _asl(Cpu* cpu, uint16_t address, bool is_accumulator){
        if (is_accumulator) {
            cpu->C = (cpu->C >> 7) & uint8_t(1);
            cpu->A <<= 1;
            cpu->setZN(cpu->A);
        } else {
            uint8_t v = cpu->Memory->Read(address);
            cpu->C = (v >> 7) & uint8_t(1);
            v <<= 1;
            cpu->Memory->Write(address, v);
            cpu->setZN(v);
        }
    }

    void _bcc(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _bcs(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _beq(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _bit(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _bmi(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _bne(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _bql(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _brk(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _bvc(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _bvs(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _clc(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _cld(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _cli(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _clv(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _cmp(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _cpx(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _cpy(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _dec(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _dex(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _dey(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _eor(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _inc(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _inx(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _iny(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _jmp(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _jsr(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _lda(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _ldx(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _ldy(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _lsr(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _nop(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _ora(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _pha(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _php(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _pla(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _plp(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _rol(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _ror(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _rti(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _rts(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _sbc(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _sec(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _sed(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _sei(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _sta(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _stx(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _sty(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _tax(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _tay(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _tsx(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _txa(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _txs(Cpu* cpu, uint16_t address, bool is_accumulator){};
    void _tya(Cpu* cpu, uint16_t address, bool is_accumulator){};

    // represents illegal operation
    void illigal(Cpu* cpu, uint16_t address, bool is_accumulator){};

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

    bool isPageDiffer(uint16_t a, uint16_t b) {
        return (a&0xFF00) == (b&0xFF00);
    }

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

    uint8_t Cpu::pull() {
        ++SP;
        return Memory->Read(uint16_t(0x100)|uint16_t(SP));
    }

    uint16_t Cpu::pull16() {
        return uint16_t(pull()) << 8 | uint16_t(pull());
    }

    void Cpu::TriggerIRQ() {
        if (I == 0) {
            interrupt = interruptIRQ;
        }
    }

    void Cpu::TriggerNMI() {
        interrupt = interruptNMI;
    }


    void Cpu::irq() {
        // TODO: implement
    }

    void Cpu::nmi() {
        // TODO: implement
    }

    // Array of instruction params to be selected by its corresponding bits.
    array<InstructionParams*, 256> InstructionTable = {};

    uint64_t Cpu::Step() {

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
            default: {
            }
        }
        // reset interrupt flag
        interrupt = interruptNone;

        // auxiliary variables
        uint64_t prev_cycles = Cycles;
        uint16_t address = 0;
        bool page_crossed = false;
        InstructionParams *inst = InstructionTable[PC];

        // switch the instruction's condition according to the addressing mode
        switch (inst->AddressingMode) {
            case Absolute : {
                address = read16(PC + uint16_t(1));
                break;
            }
            case AbsoluteX: {
                address = read16(PC + uint16_t(1)) + uint8_t(X);
                page_crossed = isPageDiffer(address - uint8_t(X), address);
                break;
            }
            case AbsoluteY: {
                address = read16(PC + uint16_t(1)) + uint8_t(Y);
                page_crossed = isPageDiffer(address - uint8_t(Y), address);
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
                page_crossed = isPageDiffer(address - uint16_t(Y), address);
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

        PC += inst->InstructionSizes;
        Cycles += inst->InstructionCycle;
        if (page_crossed) {
            Cycles += inst->PageCycle;
        }

        inst->executor(this, address, inst->AddressingMode == Accumulator);

        if (IsDebugMode) {
            cout << "[Instruction]:" << idToInstructionName.find(inst->ID)->second;
            cout << "[address]: " << address;
            cout << "[addressing mode]: " << inst->AddressingMode << "\n";
        }

        return Cycles - prev_cycles;
    }
}

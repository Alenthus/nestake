#ifndef NESTAKE_CPU
#define NESTAKE_CPU

#include <array>
#include <functional>
#include <map>
#include <stdint.h>
#include <string>

#include "memory.hpp"
#include<memory>

namespace nestake {
    class Cpu {
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

        // pointer to NesMemory interface
        std::shared_ptr<Memory> mem;

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
        void _adc(uint16_t, bool);
        void _and(uint16_t, bool);
        void _asl(uint16_t, bool);
        void _bcc(uint16_t, bool);
        void _bcs(uint16_t, bool);
        void _beq(uint16_t, bool);
        void _bit(uint16_t, bool);
        void _bmi(uint16_t, bool);
        void _bne(uint16_t, bool);
        void _bpl(uint16_t, bool);
        void _brk(uint16_t, bool);
        void _bvc(uint16_t, bool);
        void _bvs(uint16_t, bool);
        void _clc(uint16_t, bool);
        void _cld(uint16_t, bool);
        void _cli(uint16_t, bool);
        void _clv(uint16_t, bool);
        void _cmp(uint16_t, bool);
        void _cpx(uint16_t, bool);
        void _cpy(uint16_t, bool);
        void _dec(uint16_t, bool);
        void _dex(uint16_t, bool);
        void _dey(uint16_t, bool);
        void _eor(uint16_t, bool);
        void _inc(uint16_t, bool);
        void _inx(uint16_t, bool);
        void _iny(uint16_t, bool);
        void _jmp(uint16_t, bool);
        void _jsr(uint16_t, bool);
        void _lda(uint16_t, bool);
        void _ldx(uint16_t, bool);
        void _ldy(uint16_t, bool);
        void _lsr(uint16_t, bool);
        void _ora(uint16_t, bool);
        void _pha(uint16_t, bool);
        void _php(uint16_t, bool);
        void _pla(uint16_t, bool);
        void _plp(uint16_t, bool);
        void _rol(uint16_t, bool);
        void _ror(uint16_t, bool);
        void _rti(uint16_t, bool);
        void _rts(uint16_t, bool);
        void _sbc(uint16_t, bool);
        void _sec(uint16_t, bool);
        void _sed(uint16_t, bool);
        void _sei(uint16_t, bool);
        void _sta(uint16_t, bool);
        void _stx(uint16_t, bool);
        void _sty(uint16_t, bool);
        void _tax(uint16_t, bool);
        void _tay(uint16_t, bool);
        void _tsx(uint16_t, bool);
        void _txa(uint16_t, bool);
        void _txs(uint16_t, bool);
        void _tya(uint16_t, bool);

        // setup (instruction table)
        explicit Cpu(std::shared_ptr<Memory>);
    };

    // interface of instruction executor
    typedef void (*InstructionExecutor) (Cpu *cpu, uint16_t address, bool is_accumulator);

    // struct consisting of information for instruction execution
    struct InstructionParams {
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

        InstructionExecutor executor;
    };

    // map of all instructions
    std::map<uint8_t , InstructionParams> InstructionTable;
    std::map<int, std::string> idToInstructionName;

    bool isPageCrossed(uint16_t, uint16_t);
}
#endif

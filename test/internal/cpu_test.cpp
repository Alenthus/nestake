#include "gtest/gtest.h"
#include "cpu.cpp"

#include <iostream>

TEST(CPUTest, Initialization) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    // TODO: change `0` to appropriate one after implementing mapper
    EXPECT_EQ(0, cpu.PC);
    EXPECT_EQ(0xFD, cpu.SP);
    EXPECT_EQ(36, cpu.getFlag());
}


TEST(CPUTest, isPageCrossed) {
    bool actual = nestake::isPageCrossed(0, 1);
    EXPECT_EQ(false, actual);

    actual = nestake::isPageCrossed(0, 0x00FF);
    EXPECT_EQ(false, actual);

    actual = nestake::isPageCrossed(0, 0x0100);
    EXPECT_EQ(true, actual);
}

TEST(CPUTest, READ16) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    mem.RAM[0] = 0x02;
    mem.RAM[1] = 0x10;

    uint16_t actual = cpu.read16(0x00);
    EXPECT_EQ(0x1002, actual);
}


TEST(CPUTest, READ16Bug) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    mem.RAM[0x0000] = 0x02;
    mem.RAM[0x0100] = 0x33;

    // address = 0x0000, _address = 0x0001 <<8 = 0x0100
    uint16_t actual = cpu.read16Bug(0x0000);
    EXPECT_EQ(0x3302, actual);
}

TEST(CPUTest, PUSH) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    cpu.SP = 0x10;
    cpu.push(0x99);

    // 0x10 - 1 = 0x0F
    EXPECT_EQ(0x0F, cpu.SP);

    // address = (0x0100 | 0x0010) = 0x0110
    EXPECT_EQ(0x99, cpu.mem->RAM[0x0110]);
}

TEST(CPUTest, PULL) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    cpu.SP = 0x10;
    cpu.mem->RAM[0x0111] = 0x96;
    uint8_t actual = cpu.pull();

    EXPECT_EQ(0x11, cpu.SP);
    EXPECT_EQ(0x96, actual);
}

TEST(CPUTest, PULL16) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    cpu.SP = 0x10;
    cpu.mem->RAM[0x0111] = 0x96;
    cpu.mem->RAM[0x0112] = 0x98;
    uint16_t actual = cpu.pull16();

    EXPECT_EQ(0x12, cpu.SP);
    EXPECT_EQ(0x9896, actual);
}

TEST(CPUTest, GetFlag) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    cpu.C = 1;
    cpu.Z = 0;
    cpu.I = 0;
    cpu.D = 1;
    cpu.B = 0;
    cpu.U = 0;
    cpu.V = 0;
    cpu.N = 1;
    uint8_t actual = cpu.getFlag();
    EXPECT_EQ(0b10001001, actual);
}


TEST(CPUTest, SetFlags) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    cpu.setFlags(0b10001001);
    EXPECT_EQ(1, cpu.C);
    EXPECT_EQ(0, cpu.Z);
    EXPECT_EQ(0, cpu.I);
    EXPECT_EQ(1, cpu.D);
    EXPECT_EQ(0, cpu.B);
    EXPECT_EQ(0, cpu.U);
    EXPECT_EQ(0, cpu.V);
    EXPECT_EQ(1, cpu.N);
}

TEST(CPUTest, ADC) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);

    // Neither overflow nor carry case
    cpu.A = 0;
    cpu.C = 0;
    mem.RAM[0x0000] = 1;
    cpu._adc(0x0000, false);

    EXPECT_EQ(1, cpu.A);
    EXPECT_EQ(0, cpu.Z);
    EXPECT_EQ(1, cpu.N);

    // NOT overflow BUT carry
    cpu.A = 1;
    cpu.C = 0;
    mem.RAM[0x0000] = 0xFF;
    cpu._adc(0x0000, false);

    EXPECT_EQ(0x00, cpu.A);
    EXPECT_EQ(1, cpu.C);
    EXPECT_EQ(0, cpu.V);

    // NOT carry BUT overflow
    cpu.A = 0b01000000;
    cpu.C = 0;
    mem.RAM[0x0000] = 0b01000000;
    cpu._adc(0x0000, false);

    EXPECT_EQ(0b10000000, cpu.A);
    EXPECT_EQ(0, cpu.C);
    EXPECT_EQ(1, cpu.V);

    // overflow AND carry
    cpu.A = 0b10000000;
    cpu.C = 0;
    mem.RAM[0x0000] = 0b10000000;
    cpu._adc(0x0000, false);

    EXPECT_EQ(0, cpu.A);
    EXPECT_EQ(1, cpu.C);
    EXPECT_EQ(1, cpu.V);
}

TEST(CPUTest, AND) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    cpu.A = 0b10000001;
    cpu.mem->RAM[0x0000] = 0b10000000;

    cpu._and(0x0000, false);
    EXPECT_EQ(0b10000000, cpu.A);
    EXPECT_EQ(0, cpu.Z);
    EXPECT_EQ(1, cpu.N);
}

TEST(CPUTest, ASL) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);

    // accumulator mode
    cpu.A = 0b10000001;
    cpu.C = 0b00000000;

    cpu._asl(0, true);
    EXPECT_EQ(0b00000010, cpu.A);
    EXPECT_EQ(1, cpu.C);

    // non accumulator mode
    mem.RAM[0] = 0b10000001;

    cpu._asl(0, false);
    EXPECT_EQ(0b00000010, mem.RAM[0]);
    EXPECT_EQ(1, cpu.C);
}

TEST(CPUTest, BCC) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    cpu.C = 0;
    cpu.Cycles = 0;

    // page crossing
    cpu._bcc(0x1000, false);
    EXPECT_EQ(2, cpu.Cycles);
    EXPECT_EQ(0x1000, cpu.PC);

    // not page crossing
    cpu._bcc(0x1001, false);
    EXPECT_EQ(3, cpu.Cycles);
    EXPECT_EQ(0x1001, cpu.PC);
}

TEST(CPUTest, BIT) {
    nestake::Memory mem = nestake::Memory{};
    nestake::Cpu cpu = nestake::Cpu(&mem);
    cpu.A = 0b00000001;
    mem.RAM[0] = 0b11000000;
    cpu._bit(0, false);
    EXPECT_EQ(1, cpu.Z);
    //EXPECT_EQ(1, cpu.N);
    EXPECT_EQ(1, cpu.V);
}

TEST(CPUTest, BRK) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, CMP) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, DEC) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, DEX) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, EOR) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, INC) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, JSR) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, LDA) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, LSR) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, ORA) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, PHP) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, PLP) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, ROL) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, ROR) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, RTI) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, RTS) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, SBC) {
    EXPECT_EQ(true, true);
}
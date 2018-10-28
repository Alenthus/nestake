#include "gtest/gtest.h"
#include "cpu.cpp"

#include <iostream>

using std::cout;

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

TEST(CPUTest, ADC) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, AND) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, ASL) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, BCC) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, BIT) {
    EXPECT_EQ(true, true);
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
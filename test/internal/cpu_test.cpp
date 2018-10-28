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
    EXPECT_EQ(true, true);
}


TEST(CPUTest, READ16Bug) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, PUSH) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, PULL) {
    EXPECT_EQ(true, true);
}

TEST(CPUTest, PULL16) {
    EXPECT_EQ(true, true);
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
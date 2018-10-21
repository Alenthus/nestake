#include "gtest/gtest.h"
#include "cpu.cpp"

TEST(CPUTest, TestGetSampler1) {
    int actual = add(10, 10);
    EXPECT_EQ(20, actual);
}
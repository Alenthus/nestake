#include "gtest/gtest.h"
#include "ines.cpp"

#include <iostream>

TEST(INESTEST, Initialization) {
    std::string path = "../../resources/read_test.nes";
    nestake::Cartridge c = nestake::Cartridge{path};
}

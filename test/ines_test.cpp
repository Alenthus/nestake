#include "gtest/gtest.h"
#include "ines.cpp"

#include <iostream>

TEST(INESTEST, Initialization) {
    std::string path = "../../resources/sample.nes";
    nestake::Cartridge c = nestake::Cartridge{path};
}

#include "gtest/gtest.h"
#include "console.cpp"

#include <iostream>


TEST(ConsoleTest, Step) {
    const std::string path = "../../resources/sample.nes";
    std::shared_ptr<nestake::CPUMemory> mem(std::make_shared<nestake::CPUMemory>());
    std::shared_ptr<nestake::Cpu> cpu(std::make_shared<nestake::Cpu>(mem));
    std::shared_ptr<nestake::Cartridge> cart(std::make_shared<nestake::Cartridge>(path));
    std::shared_ptr<nestake::Console> console(std::make_shared<nestake::Console>(cpu, cart));
    console->Step();
}

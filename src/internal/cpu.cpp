#include <string>
#include "cpu.hpp"

// Addressing mode struct for expressing each addressing mode as int
struct AddressingMode {
    const std::string Absolute = "Absolute";
    const std::string AbsoluteX = "Absolute";
    const std::string AbsoluteY = "AbsoluteY";
    const std::string Accumulator = "Accumulator";
    const std::string Immediate = "Immediate";
    const std::string Implied = "Implied";
    const std::string IndexedIndirect = "IndexedIndirect";
    const std::string Indirect = "Indirect";
    const std::string IndirectIndexed = "IndirectIndexed";
    const std::string Relative = "Relative";
    const std::string ZeroPage = "ZeroPage";
    const std::string ZeroPageX = "ZeroPageX";
    const std::string ZeroPageY = "ZeroPageY";
};

// type of interruption in Int
struct InterruptType {
    static const int interruptNone = 1;
    static const int interruptNMI = 2;
    static const int interruptIRQ = 3;
};

/*
 *  Array of instruction params to be selected by its corresponding bits.
 */
std::array<InstructionParams, 256> InstructionTable = {};
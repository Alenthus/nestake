#include <string>
#include "cpu.hpp"

// Addressing mode struct for expressing each addressing mode as int

using std::string;
using std::array;

namespace cpu {
    struct InstructionName {
        const string ADC = "ADC";
        const string AHX = "AHX";
        const string ALR = "ALR";
        const string ANC = "ANC";
        const string AND = "AND";
        const string ARR = "ARR";
        const string ASL = "ASL";
        const string AXS = "AXS";
        const string BCC = "BCC";
        const string BCS = "BCS";
        const string BEQ = "BEQ";
        const string BIT = "BIT";
        const string BMI = "BMI";
        const string BNE = "BNE";
        const string BPL = "BPL";
        const string BRK = "BRK";
        const string BVC = "BVC";
        const string BVS = "BVS";
        const string CLC = "CLC";
        const string CLD = "CLD";
        const string CLI = "CLI";
        const string CLV = "CLV";
        const string CMP = "CMP";
        const string CPX = "CPX";
        const string CPY = "CPY";
        const string DCP = "DCP";
        const string DEC = "DEC";
        const string DEX = "DEX";
        const string DEY = "DEY";
        const string EOR = "EOR";
        const string INC = "INC";
        const string INX = "INX";
        const string INY = "INY";
        const string ISC = "ISC";
        const string JMP = "JMP";
        const string JSR = "JSR";
        const string KIL = "KIL";
        const string LAS = "LAS";
        const string LAX = "LAX";
        const string LDA = "LDA";
        const string LDX = "LDX";
        const string LDY = "LDY";
        const string LSR = "LSR";
        const string NOP = "NOP";
        const string ORA = "ORA";
        const string PHA = "PHA";
        const string PHP = "PHP";
        const string PLA = "PLA";
        const string PLP = "PLP";
        const string RLA = "RLA";
        const string ROL = "ROL";
        const string ROR = "ROR";
        const string RRA = "RRA";
        const string RTI = "RTI";
        const string RTS = "RTS";
        const string SAX = "SAX";
        const string SBC = "SBC";
        const string SEC = "SEC";
        const string SED = "SED";
        const string SEI = "SEI";
        const string SHX = "SHX";
        const string SHY = "SHY";
        const string SLO = "SLO";
        const string SRE = "SRE";
        const string STA = "STA";
        const string STX = "STX";
        const string STY = "STY";
        const string TAS = "TAS";
        const string TAX = "TAX";
        const string TAY = "TAY";
        const string TSX = "TSX";
        const string TXA = "TXA";
        const string TXS = "TXS";
        const string TYA = "TYA";
        const string XAA = "XAA";
    };

    enum AddressingMode {
        Absolute = 1,
        AbsoluteX,
        AbsoluteY,
        Accumulator ,
        Immediate,
        Implied,
        IndexedIndirect,
        Indirect,
        IndirectIndexed,
        Relative,
        ZeroPage,
        ZeroPageX,
        ZeroPageY,
    };

    // type of interruption
    enum InterruptType {
        interruptNone = 1,
        interruptNMI,
        interruptIRQ,
    };

    /*
     *  Array of instruction params to be selected by its corresponding bits.
     */
    array<InstructionParams, 256> InstructionTable = {};
}

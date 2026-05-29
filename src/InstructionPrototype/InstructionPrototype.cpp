#include "InstructionPrototype.hpp"

std::size_t InstructionPrototype::size() const {
    std::size_t sizeBits = opcodeBitWidth;

    for (const auto &operand : operands) {
        sizeBits += getOperandBitWidth(operand);
    }

    return (sizeBits + 7) / 8;
}

InstructionPrototype::InstructionPrototype(
    std::string format,
    std::uint16_t opcode,
    std::uint8_t opcodeSize,
    std::vector<OperandType> operands
) :
    format(format),
    opcode(opcode),
    opcodeBitWidth(opcodeSize),
    operands(operands) {}

bool InstructionPrototype::doesMatch(const std::vector<std::uint8_t> &bytes
) const {
    if (bytes.size() != size()) {
        return false;
    }

    std::size_t opcodeBytes = (opcodeBitWidth + 7) / 8;
    const std::size_t backshift = opcodeBytes * 8 - opcodeBitWidth;
    std::uint16_t extractedOpcode = 0;

    for (std::size_t i = 0; i < opcodeBytes; i++) {
        extractedOpcode <<= 8;
        extractedOpcode |= bytes[i];
    }

    extractedOpcode >>= backshift;

    if (extractedOpcode != opcode) {
        return false;
    }

    return true;
}
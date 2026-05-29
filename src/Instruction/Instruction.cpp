#include "Instruction.hpp"

Instruction::Instruction(
    const InstructionPrototype &prototype, std::vector<std::uint8_t> bytes
) :
    prototype(prototype), bytes(bytes) {}

std::string Instruction::toAsm() const {
    std::string result = prototype.format;
    std::size_t cumulativeBitOffset = prototype.opcodeBitWidth;

    for (std::size_t i = 0; i < prototype.operands.size(); i++) {
        std::string operandWildcard = '$' + std::to_string(i + 1);
        std::size_t position = result.find(operandWildcard);
        bool isRegisterInLowerNibble = cumulativeBitOffset % 8 != 0;

        if (position == std::string::npos) {
            break;
        }

        result.replace(
            position,
            operandWildcard.length(),
            formatOperandBytes(
                getOperandBytes(i),
                prototype.operands[i],
                isRegisterInLowerNibble
            )
        );

        cumulativeBitOffset += getOperandBitWidth(prototype.operands[i]);
    }

    return result;
}

std::vector<std::uint8_t> Instruction::getOperandBytes(int operandIndex) const {
    std::size_t offsetBits = prototype.opcodeBitWidth;

    for (int i = 0; i < operandIndex; i++) {
        offsetBits += getOperandBitWidth(prototype.operands[i]);
    }

    std::size_t operandBits =
        getOperandBitWidth(prototype.operands[operandIndex]);
    std::size_t operandBytes = (operandBits + 7) / 8;
    std::vector<std::uint8_t> result(operandBytes);

    for (std::size_t i = 0; i < operandBytes; i++) {
        std::size_t bitIndex = offsetBits + i * 8;
        std::size_t byteIndex = bitIndex / 8;

        result[i] = bytes[byteIndex];
    }

    return result;
}
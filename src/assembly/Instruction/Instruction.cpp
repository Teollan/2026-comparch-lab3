#include "assembly/Instruction/Instruction.hpp"

#include <utility>

Instruction::Instruction(
    const InstructionPrototype& prototype,
    std::vector<std::uint8_t> bytes,
    std::size_t offset,
    const Vram& vram
) :
    prototype(prototype), _bytes(std::move(bytes)), _offset(offset) {
    translate(vram);
}

std::size_t Instruction::offset() const {
    return _offset;
}

std::size_t Instruction::size() const {
    return _bytes.size();
}

std::optional<TranslationFailure> Instruction::error() const {
    return _error;
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

        result[i] = _bytes[byteIndex];
    }

    bool isRegister = prototype.operands[operandIndex] == OperandType::Register;

    if (isRegister) {
        if (offsetBits % 8 == 0) {
            result[0] >>= 4;
        }

        result[0] &= 0x0F;
    }

    return result;
}

void Instruction::translate(const Vram& vram) {
    for (std::size_t i = 0; i < prototype.operands.size(); i++) {
        if (prototype.operands[i] != OperandType::Address) {
            continue;
        }

        std::vector<std::uint8_t> addressBytes = getOperandBytes(i);
        std::uint32_t virtualAddress =
            (std::uint32_t(addressBytes[0]) << 24) |
            (std::uint32_t(addressBytes[1]) << 16) |
            (std::uint32_t(addressBytes[2]) << 8) |
            std::uint32_t(addressBytes[3]);

        auto physical = vram.virtualToPhysical(virtualAddress);
        if (!physical) {
            _error = TranslationFailure{virtualAddress, physical.error()};
            return;
        }

        setAddressBytes(i, *physical);
        return;
    }
}

void Instruction::setAddressBytes(int operandIndex, std::uint32_t value) {
    std::size_t offsetBits = prototype.opcodeBitWidth;

    for (int i = 0; i < operandIndex; i++) {
        offsetBits += getOperandBitWidth(prototype.operands[i]);
    }

    std::size_t operandBytes = (getOperandBitWidth(OperandType::Address) + 7) / 8;

    for (std::size_t i = 0; i < operandBytes; i++) {
        std::size_t byteIndex = (offsetBits + i * 8) / 8;

        _bytes[byteIndex] = (value >> (8 * (operandBytes - 1 - i))) & 0xFF;
    }
}

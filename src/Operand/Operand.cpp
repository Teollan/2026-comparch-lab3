#include "Operand.hpp"

#include <bit>
#include <format>

std::string formatOperandBytes(
    const std::vector<std::uint8_t> &bytes,
    OperandType type,
    bool isRegisterInLowerNibble
) {
    switch (type) {
        case OperandType::Register:
            return formatRegister(bytes[0], isRegisterInLowerNibble);
        case OperandType::Address:
            return formatAddress(bytes);
        case OperandType::Literal16:
            return formatLiteral16(bytes);
        case OperandType::Literal32:
            return formatLiteral32(bytes);
        default:
            throw std::invalid_argument("Unknown operand type");
    }
}

std::string formatRegister(std::uint8_t byte, bool isRegisterInLowerNibble) {
    if (!isRegisterInLowerNibble) {
        byte >>= 4;
    }

    return std::format("R{}", byte & 0x0F);
}

std::string formatAddress(const std::vector<std::uint8_t> &bytes) {
    std::uint32_t addres =
        (std::uint32_t(bytes[0]) << 24) | (std::uint32_t(bytes[1]) << 16) |
        (std::uint32_t(bytes[2]) << 8) | std::uint32_t(bytes[3]);

    return std::format("[0x{:08X}]", addres);
}

std::string formatLiteral16(const std::vector<std::uint8_t> &bytes) {
    std::uint16_t raw =
        (std::uint16_t(bytes[0]) << 8) | std::uint16_t(bytes[1]);

    return std::format("{}", std::bit_cast<std::int16_t>(raw));
}

std::string formatLiteral32(const std::vector<std::uint8_t> &bytes) {
    std::uint32_t raw =
        (std::uint32_t(bytes[0]) << 24) | (std::uint32_t(bytes[1]) << 16) |
        (std::uint32_t(bytes[2]) << 8) | std::uint32_t(bytes[3]);

    return std::format("{}", std::bit_cast<std::int32_t>(raw));
}

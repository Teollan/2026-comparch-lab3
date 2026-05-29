#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

enum class OperandType : std::uint8_t {
    Register,
    Address,
    Literal16,
    Literal32,
};

std::string formatOperandBytes(
    const std::vector<std::uint8_t> &bytes,
    OperandType type,
    bool isRegisterInLowerNibble = true  // I hate this so much
);

std::string formatRegister(
    std::uint8_t byte, bool isRegisterInLowerNibble = true
);

std::string formatAddress(const std::vector<std::uint8_t> &bytes);

std::string formatLiteral16(const std::vector<std::uint8_t> &bytes);

std::string formatLiteral32(const std::vector<std::uint8_t> &bytes);

constexpr std::uint8_t getOperandBitWidth(OperandType type) {
    switch (type) {
        case OperandType::Register:
            return 4;
        case OperandType::Address:
            return 32;
        case OperandType::Literal16:
            return 16;
        case OperandType::Literal32:
            return 32;
        default:
            throw std::invalid_argument("Unknown operand type");
    }
}

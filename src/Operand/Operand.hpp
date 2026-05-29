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
    bool isRegisterInLowerNibble = true  // I hate this
);

std::string formatRegister(
    std::uint8_t byte, bool isRegisterInLowerNibble = true
);

std::string formatAddress(const std::vector<std::uint8_t> &bytes);

std::string formatLiteral16(const std::vector<std::uint8_t> &bytes);

std::string formatLiteral32(const std::vector<std::uint8_t> &bytes);

std::uint8_t getOperandBitWidth(OperandType type);

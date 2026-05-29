#pragma once

#include <InstructionPrototype.hpp>
#include <cstdint>
#include <optional>
#include <vector>

class Instruction {
public:
    const InstructionPrototype &prototype;
    std::vector<std::uint8_t> bytes;

    Instruction(
        const InstructionPrototype &prototype, std::vector<std::uint8_t> bytes
    );

    std::string toAsm() const;
    std::vector<std::uint8_t> getOperandBytes(int operandIndex) const;
};
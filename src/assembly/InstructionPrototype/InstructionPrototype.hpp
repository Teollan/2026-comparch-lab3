#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "assembly/Operand/Operand.hpp"

class InstructionPrototype {
public:
    const std::string format;
    const std::uint16_t opcode;
    const std::uint8_t opcodeBitWidth;
    const std::vector<OperandType> operands;

    std::size_t size() const;

    InstructionPrototype(
        std::string format,
        std::uint16_t opcode,
        std::uint8_t opcodeBitWidth,
        std::vector<OperandType> operands = {}
    );

    bool doesMatch(const std::vector<std::uint8_t>& bytes) const;
};

const std::vector<InstructionPrototype> INSTRUCTION_PROTOTYPES = {
    InstructionPrototype(
        "MOV $1, $2",
        0x1B0,
        12,
        {OperandType::Register, OperandType::Address}
    ),
    InstructionPrototype(
        "MOV $2, $1",
        0x1B1,
        12,
        {OperandType::Register, OperandType::Address}
    ),
    InstructionPrototype(
        "ADD $1, $2, $3",
        0x030,
        12,
        {OperandType::Register, OperandType::Register, OperandType::Register}
    ),
    InstructionPrototype(
        "ADD $1, $2, $3",
        0x04,
        8,
        {OperandType::Register, OperandType::Register, OperandType::Address}
    ),
    InstructionPrototype(
        "SUB $1, $2",
        0x0A,
        8,
        {OperandType::Register, OperandType::Register}
    ),
    InstructionPrototype(
        "SUB $1, $2",
        0x0B0,
        12,
        {OperandType::Register, OperandType::Address}
    ),
    InstructionPrototype(
        "CMP $1, $2",
        0x80,
        8,
        {OperandType::Register, OperandType::Register}
    ),
    InstructionPrototype(
        "MOV $1, $2",
        0x1C1,
        12,
        {OperandType::Register, OperandType::Literal16}
    ),
    InstructionPrototype(
        "MOV $1, $2",
        0x1C2,
        12,
        {OperandType::Register, OperandType::Literal32}
    ),
};
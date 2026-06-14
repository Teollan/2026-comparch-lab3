#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include "assembly/InstructionPrototype/InstructionPrototype.hpp"
#include "vram/Vram/Vram.hpp"

class Instruction {
public:
    const InstructionPrototype& prototype;

    Instruction(
        const InstructionPrototype& prototype,
        std::vector<std::uint8_t> bytes,
        std::size_t offset,
        const Vram& vram
    );

    std::size_t offset() const;
    std::size_t size() const;
    std::optional<TranslationFailure> error() const;

    std::vector<std::uint8_t> getOperandBytes(int operandIndex) const;

private:
    std::vector<std::uint8_t> _bytes;
    std::size_t _offset;
    std::optional<TranslationFailure> _error;

    void translate(const Vram& vram);
    void setAddressBytes(int operandIndex, std::uint32_t value);
};

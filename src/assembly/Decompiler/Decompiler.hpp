#pragma once

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

#include "assembly/Instruction/Instruction.hpp"
#include "vram/Vram/Vram.hpp"

std::vector<Instruction> decompile(
    std::vector<std::uint8_t>& bytes,
    const Vram& vram
);

class DecompilerUnknownInstructionException : public std::exception {
private:
    std::size_t offset;

public:
    DecompilerUnknownInstructionException(std::size_t offset);
};
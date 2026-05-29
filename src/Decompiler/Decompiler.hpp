#pragma once

#include <Instruction.hpp>
#include <cstdint>
#include <vector>

std::vector<Instruction> decompile(std::vector<std::uint8_t> &bytes);

class DecompilerUnknownInstructionException : public std::exception {
private:
    std::size_t offset;

public:
    DecompilerUnknownInstructionException(std::size_t offset);
};
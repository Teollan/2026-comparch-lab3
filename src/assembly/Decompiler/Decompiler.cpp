#include "assembly/Decompiler/Decompiler.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

std::vector<Instruction> decompile(
    std::vector<std::uint8_t>& bytes,
    const Vram& vram
) {
    std::vector<Instruction> instructions;

    auto iterator = bytes.begin();
    while (iterator != bytes.end()) {
        std::size_t offset = iterator - bytes.begin();
        std::optional<Instruction> instruction = std::nullopt;

        for (const auto& prototype : INSTRUCTION_PROTOTYPES) {
            auto instructionBytes = std::vector<std::uint8_t>(
                iterator, iterator + prototype.size()
            );

            if (prototype.doesMatch(instructionBytes)) {
                instruction.emplace(
                    prototype, std::move(instructionBytes), offset, vram
                );

                break;
            }
        }

        if (!instruction) {
            throw DecompilerUnknownInstructionException(offset);
        }

        iterator += instruction->size();
        instructions.push_back(std::move(*instruction));
    };

    return instructions;
}

DecompilerUnknownInstructionException::DecompilerUnknownInstructionException(
    std::size_t offset
) :
    offset(offset) {}

#include "Decompiler.hpp"

std::vector<Instruction> decompile(std::vector<std::uint8_t> &bytes) {
    std::vector<Instruction> instructions;

    auto iterator = bytes.begin();
    while (iterator != bytes.end()) {
        std::optional<Instruction> instruction = std::nullopt;

        for (const auto &prototype : INSTRUCTION_PROTOTYPES) {
            auto instructionBytes = std::vector<std::uint8_t>(
                iterator, iterator + prototype.size()
            );

            if (prototype.doesMatch(instructionBytes)) {
                instruction.emplace(prototype, instructionBytes);
            }
        }

        if (!instruction) {
            throw DecompilerUnknownInstructionException(
                iterator - bytes.begin()
            );
        }

        instructions.push_back(instruction.value());
        iterator += instruction->bytes.size();
    };

    return instructions;
}

DecompilerUnknownInstructionException::DecompilerUnknownInstructionException(
    std::size_t offset
) :
    offset(offset) {}
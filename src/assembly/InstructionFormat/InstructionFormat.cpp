#include "assembly/InstructionFormat/InstructionFormat.hpp"

#include "assembly/Operand/Operand.hpp"

std::string formatInstruction(const Instruction& instruction) {
    const InstructionPrototype& prototype = instruction.prototype;

    std::string result = prototype.format;

    for (std::size_t i = 0; i < prototype.operands.size(); i++) {
        std::string operandWildcard = '$' + std::to_string(i + 1);
        std::size_t position = result.find(operandWildcard);

        if (position == std::string::npos) {
            break;
        }

        result.replace(
            position,
            operandWildcard.length(),
            formatOperandBytes(
                instruction.getOperandBytes(i), prototype.operands[i]
            )
        );
    }

    return result;
}

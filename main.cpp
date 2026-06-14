#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "assembly/Decompiler/Decompiler.hpp"
#include "assembly/Instruction/Instruction.hpp"
#include "assembly/InstructionFormat/InstructionFormat.hpp"
#include "util/Args/Args.hpp"
#include "util/Hex/Hex.hpp"
#include "vram/VramImage/VramImage.hpp"

std::optional<Args> tryParseArgs(int argc, char* argv[]) {
    Args args(argc, argv);
    args.expectString("input", "i", true)
        .expectString("segments-table", "s", true)
        .expectFlag("hex", "x")
        .expectString("output", "o");

    try {
        args.parse();
    } catch (const ArgsException& e) {
        std::cerr << e.what() << "\n";
        args.printUsage();
        return std::nullopt;
    }

    return args;
}

std::optional<std::vector<std::uint8_t>> tryReadInput(const Args& args) {
    std::ifstream input(args.getString("input"), std::ios::binary);
    if (!input) {
        std::cerr << "Failed to open file: " << args.getString("input") << "\n";
        return std::nullopt;
    }

    if (args.getFlag("hex")) {
        std::stringstream buffer;
        buffer << input.rdbuf();
        return hexToBytes(buffer.str());
    } else {
        std::vector<std::uint8_t> bytes;
        bytes.assign(
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()
        );
        return bytes;
    }
}

std::optional<Vram> tryLoadVramImage(const Args& args) {
    try {
        return VramImage::load(args.getString("segments-table"));
    } catch (const std::exception& e) {
        std::cerr << "Failed to load VRAM image: " << e.what() << "\n";
        return std::nullopt;
    }
}

std::optional<std::vector<Instruction>> tryDisassembling(
    std::vector<std::uint8_t>& bytes,
    const Vram& vram
) {
    try {
        return decompile(bytes, vram);
    } catch (const std::exception& e) {
        std::cerr << "Failed to disassemble bytes: " << e.what() << "\n";
        return std::nullopt;
    }
}

int main(int argc, char* argv[]) {
    auto args = tryParseArgs(argc, argv);
    if (!args) {
        return 1;
    }

    auto bytes = tryReadInput(*args);
    if (!bytes) {
        return 1;
    }

    auto vram = tryLoadVramImage(*args);
    if (!vram) {
        return 1;
    }

    auto instructions = tryDisassembling(*bytes, *vram);
    if (!instructions) {
        return 1;
    }

    std::ofstream outputFile;
    std::ostream* out = &std::cout;

    const auto& outputPath = args->getString("output");
    if (!outputPath.empty()) {
        outputFile.open(outputPath);

        if (!outputFile) {
            std::cerr << "Failed to open output: " << outputPath << "\n";
            return 1;
        }

        out = &outputFile;
    }

    for (const auto& instruction : *instructions) {
        *out << bytesToHex(*bytes, instruction.offset(), instruction.size())
             << "\n";

        if (auto failure = instruction.error()) {
            *out << "ERROR: "
                 << describeTranslationError(
                        failure->virtualAddress, failure->error
                    )
                 << "\n";
        }

        *out << formatInstruction(instruction) << "\n\n";
    }

    return 0;
}

#include <Args.hpp>
#include <Decompiler.hpp>
#include <Hex.hpp>
#include <Instruction.hpp>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

int main(int argc, char *argv[]) {
    Args args(argc, argv);
    args.expectString("input", "i", true)
        .expectFlag("hex", "x")
        .expectString("output", "o");

    try {
        args.parse();
    } catch (const ArgsException &e) {
        std::cerr << e.what() << "\n";
        args.printUsage();
        return 1;
    }

    std::ifstream input(args.getString("input"), std::ios::binary);
    if (!input) {
        std::cerr << "Failed to open input: " << args.getString("input")
                  << "\n";
        return 1;
    }

    std::vector<std::uint8_t> bytes;
    if (args.getFlag("hex")) {
        std::stringstream buffer;
        buffer << input.rdbuf();
        bytes = hexToBytes(buffer.str());
    } else {
        bytes.assign(
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()
        );
    }

    auto instructions = decompile(bytes);

    std::ofstream outputFile;
    std::ostream *out = &std::cout;
    if (!args.getString("output").empty()) {
        outputFile.open(args.getString("output"));
        if (!outputFile) {
            std::cerr << "Failed to open output: " << args.getString("output")
                      << "\n";
            return 1;
        }
        out = &outputFile;
    }

    for (const auto &instruction : instructions) {
        *out << bytesToHex(instruction.bytes) << "\n"
             << instruction.toAsm() << "\n\n";
    }

    return 0;
}

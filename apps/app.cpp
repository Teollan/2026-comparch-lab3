#include <Decompiler.hpp>
#include <Hex.hpp>
#include <Instruction.hpp>
#include <iostream>

int main() {
    auto bytes = hexToBytes(
        "1B 01 00 00 00 42 "        // MOV  R1, [0x00000042]        (template 2)
        "1B 12 00 00 00 42 "        // MOV  [0x00000042], R2        (template 3)
        "03 01 23 "                 // ADD  R1, R2, R3              (template 6)
        "04 12 00 00 00 42 "        // ADD  R1, R2, [0x00000042]    (template 7)
        "0A 12 "                    // SUB  R1, R2                  (template 8)
        "0B 01 00 00 00 42 "        // SUB  R1, [0x00000042]        (template 9)
        "80 AF "                    // CMP  R10, R15                (template 26)
        "1C 11 33 44 "              // MOV  R1, 13124               (template 28)
        "1C 1F FF FF "              // MOV  R15, -1                 (template 28, signed)
        "1C 21 33 44 55 66 "        // MOV  R1, 860116326           (template 29)
        "1C 2F FF FF FF FF"         // MOV  R15, -1                 (template 29, signed)
    );

    auto instructions = decompile(bytes);

    for (const auto &instruction : instructions) {
        std::cout << bytesToHex(instruction.bytes) << "\n"
                  << instruction.toAsm() << "\n\n";
    }

    return 0;
}

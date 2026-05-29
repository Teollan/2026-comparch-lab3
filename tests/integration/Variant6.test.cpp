#include <gtest/gtest.h>

#include <Decompiler.hpp>
#include <Hex.hpp>
#include <Instruction.hpp>
#include <string>
#include <vector>

namespace {

std::string decodeSingle(std::string_view hex) {
    auto bytes = hexToBytes(hex);
    auto instructions = decompile(bytes);
    return instructions[0].toAsm();
}

std::vector<std::string> decodeAll(std::string_view hex) {
    auto bytes = hexToBytes(hex);
    auto instructions = decompile(bytes);

    std::vector<std::string> result;
    for (const auto &instr : instructions) {
        result.push_back(instr.toAsm());
    }

    return result;
}

}  // namespace

TEST(Variant6, Template2_MovRegFromAddr) {
    EXPECT_EQ(decodeSingle("1B 01 00 00 00 42"), "MOV R1, [0x00000042]");
}

TEST(Variant6, Template3_MovAddrFromReg) {
    EXPECT_EQ(decodeSingle("1B 12 00 00 00 42"), "MOV [0x00000042], R2");
}

TEST(Variant6, Template6_AddRegRegReg) {
    EXPECT_EQ(decodeSingle("03 01 23"), "ADD R1, R2, R3");
}

TEST(Variant6, Template7_AddRegRegAddr) {
    EXPECT_EQ(decodeSingle("04 12 00 00 00 42"), "ADD R1, R2, [0x00000042]");
}

TEST(Variant6, Template8_SubRegReg) {
    EXPECT_EQ(decodeSingle("0A 12"), "SUB R1, R2");
}

TEST(Variant6, Template9_SubRegAddr) {
    EXPECT_EQ(decodeSingle("0B 01 00 00 00 42"), "SUB R1, [0x00000042]");
}

TEST(Variant6, Template26_CmpRegReg) {
    EXPECT_EQ(decodeSingle("80 AF"), "CMP R10, R15");
}

TEST(Variant6, Template28_MovRegLit16Positive) {
    EXPECT_EQ(decodeSingle("1C 11 33 44"), "MOV R1, 13124");
}

TEST(Variant6, Template28_MovRegLit16Negative) {
    EXPECT_EQ(decodeSingle("1C 1F FF FF"), "MOV R15, -1");
}

TEST(Variant6, Template29_MovRegLit32Positive) {
    EXPECT_EQ(decodeSingle("1C 21 33 44 55 66"), "MOV R1, 860116326");
}

TEST(Variant6, Template29_MovRegLit32Negative) {
    EXPECT_EQ(decodeSingle("1C 2F FF FF FF FF"), "MOV R15, -1");
}

TEST(Variant6, AllTemplatesInSingleStream) {
    auto asm_lines = decodeAll(
        "1B 01 00 00 00 42 "  // template 2  : MOV R1, [0x00000042]
        "1B 12 00 00 00 42 "  // template 3  : MOV [0x00000042], R2
        "03 01 23 "           // template 6  : ADD R1, R2, R3
        "04 12 00 00 00 42 "  // template 7  : ADD R1, R2, [0x00000042]
        "0A 12 "              // template 8  : SUB R1, R2
        "0B 01 00 00 00 42 "  // template 9  : SUB R1, [0x00000042]
        "80 AF "              // template 26 : CMP R10, R15
        "1C 11 33 44 "        // template 28 : MOV R1, 13124
        "1C 21 33 44 55 66"   // template 29 : MOV R1, 860116326
    );

    EXPECT_EQ(
        asm_lines,
        (std::vector<std::string>{
            "MOV R1, [0x00000042]",
            "MOV [0x00000042], R2",
            "ADD R1, R2, R3",
            "ADD R1, R2, [0x00000042]",
            "SUB R1, R2",
            "SUB R1, [0x00000042]",
            "CMP R10, R15",
            "MOV R1, 13124",
            "MOV R1, 860116326",
        })
    );
}

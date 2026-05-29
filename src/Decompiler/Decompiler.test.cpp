#include "Decompiler.hpp"

#include <gtest/gtest.h>

#include <vector>

using Bytes = std::vector<std::uint8_t>;

TEST(decompile, EmptyInputReturnsEmptyResult) {
    Bytes bytes;
    auto result = decompile(bytes);

    EXPECT_TRUE(result.empty());
}

TEST(decompile, SingleTwoByteInstruction) {
    Bytes bytes{0x0A, 0x12};
    auto result = decompile(bytes);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].bytes, bytes);
    EXPECT_EQ(result[0].prototype.opcode, 0x0A);
    EXPECT_EQ(result[0].toAsm(), "SUB R1, R2");
}

TEST(decompile, SingleThreeByteInstruction) {
    Bytes bytes{0x03, 0x01, 0x23};
    auto result = decompile(bytes);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].prototype.opcode, 0x030);
    EXPECT_EQ(result[0].toAsm(), "ADD R1, R2, R3");
}

TEST(decompile, SingleSixByteInstruction) {
    Bytes bytes{0x1B, 0x01, 0x00, 0x00, 0x00, 0x42};
    auto result = decompile(bytes);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].prototype.opcode, 0x1B0);
    EXPECT_EQ(result[0].toAsm(), "MOV R1, [0x00000042]");
}

TEST(decompile, MultipleInstructionsSameSize) {
    Bytes bytes{
        0x0A,
        0x12,
        0x80,
        0xAF,
    };
    auto result = decompile(bytes);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].toAsm(), "SUB R1, R2");
    EXPECT_EQ(result[1].toAsm(), "CMP R10, R15");
}

TEST(decompile, MultipleInstructionsMixedSizes) {
    Bytes bytes{
        0x0A,
        0x12,  // 2 bytes: SUB R1, R2
        0x03,
        0x01,
        0x23,  // 3 bytes: ADD R1, R2, R3
        0x1B,
        0x01,
        0x00,
        0x00,
        0x00,
        0x42,  // 6 bytes: MOV R1, [...]
        0x80,
        0xAF,  // 2 bytes: CMP R10, R15
    };
    auto result = decompile(bytes);

    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0].toAsm(), "SUB R1, R2");
    EXPECT_EQ(result[1].toAsm(), "ADD R1, R2, R3");
    EXPECT_EQ(result[2].toAsm(), "MOV R1, [0x00000042]");
    EXPECT_EQ(result[3].toAsm(), "CMP R10, R15");
}

TEST(decompile, DistinguishesNibbleVariantOpcodes) {
    // 1B 0 ... -> MOV reg, addr
    // 1B 1 ... -> MOV addr, reg (operand order swapped)
    Bytes bytes{
        0x1B,
        0x01,
        0x00,
        0x00,
        0x00,
        0x42,
        0x1B,
        0x12,
        0x00,
        0x00,
        0x00,
        0x42,
    };
    auto result = decompile(bytes);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].prototype.opcode, 0x1B0);
    EXPECT_EQ(result[0].toAsm(), "MOV R1, [0x00000042]");
    EXPECT_EQ(result[1].prototype.opcode, 0x1B1);
    EXPECT_EQ(result[1].toAsm(), "MOV [0x00000042], R2");
}

TEST(decompile, DistinguishesLit16AndLit32) {
    // 1C 1 ... -> 4-byte form (lit16)
    // 1C 2 ... -> 6-byte form (lit32)
    Bytes bytes{
        0x1C,
        0x11,
        0x33,
        0x44,
        0x1C,
        0x21,
        0x33,
        0x44,
        0x55,
        0x66,
    };
    auto result = decompile(bytes);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].prototype.opcode, 0x1C1);
    EXPECT_EQ(result[0].bytes.size(), 4);
    EXPECT_EQ(result[1].prototype.opcode, 0x1C2);
    EXPECT_EQ(result[1].bytes.size(), 6);
}

TEST(decompile, ThrowsOnUnknownOpcode) {
    // 0xFF is not a recognized opcode prefix
    Bytes bytes{0xFF, 0x00};

    EXPECT_THROW(decompile(bytes), DecompilerUnknownInstructionException);
}

TEST(decompile, ThrowsAfterDecodingValidPrefix) {
    // Decode the SUB first, then choke on unknown 0xFF
    Bytes bytes{0x0A, 0x12, 0xFF, 0x00};

    EXPECT_THROW(decompile(bytes), DecompilerUnknownInstructionException);
}

TEST(decompile, BytesFieldOnInstructionMatchesInputSlice) {
    Bytes bytes{0x1B, 0x01, 0x00, 0x00, 0x00, 0x42, 0x0A, 0x12};
    auto result = decompile(bytes);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].bytes, (Bytes{0x1B, 0x01, 0x00, 0x00, 0x00, 0x42}));
    EXPECT_EQ(result[1].bytes, (Bytes{0x0A, 0x12}));
}

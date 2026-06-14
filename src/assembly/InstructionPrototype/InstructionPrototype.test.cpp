#include "assembly/InstructionPrototype/InstructionPrototype.hpp"

#include <gtest/gtest.h>

TEST(InstructionPrototype, ConstructorStoresFields) {
    InstructionPrototype proto(
        "MOV $1, $2", 0x1B0, 12, {OperandType::Register, OperandType::Address}
    );

    EXPECT_EQ(proto.format, "MOV $1, $2");
    EXPECT_EQ(proto.opcode, 0x1B0);
    EXPECT_EQ(proto.opcodeBitWidth, 12);
    EXPECT_EQ(proto.operands.size(), 2);
}

TEST(InstructionPrototype, SizeEightBitOpcodeTwoRegs) {
    // 8 + 4 + 4 = 16 bits = 2 bytes
    InstructionPrototype proto(
        "SUB $1, $2", 0x0A, 8, {OperandType::Register, OperandType::Register}
    );

    EXPECT_EQ(proto.size(), 2);
}

TEST(InstructionPrototype, SizeTwelveBitOpcodeWithAddress) {
    // 12 + 4 + 32 = 48 bits = 6 bytes
    InstructionPrototype proto(
        "MOV $1, $2", 0x1B0, 12, {OperandType::Register, OperandType::Address}
    );

    EXPECT_EQ(proto.size(), 6);
}

TEST(InstructionPrototype, SizeTwelveBitOpcodeThreeRegs) {
    // 12 + 4 + 4 + 4 = 24 bits = 3 bytes
    InstructionPrototype proto(
        "ADD $1, $2, $3",
        0x030,
        12,
        {OperandType::Register, OperandType::Register, OperandType::Register}
    );

    EXPECT_EQ(proto.size(), 3);
}

TEST(InstructionPrototype, SizeWithLit16) {
    // 12 + 4 + 16 = 32 bits = 4 bytes
    InstructionPrototype proto(
        "MOV $1, $2", 0x1C1, 12, {OperandType::Register, OperandType::Literal16}
    );

    EXPECT_EQ(proto.size(), 4);
}

TEST(InstructionPrototype, SizeWithLit32) {
    // 12 + 4 + 32 = 48 bits = 6 bytes
    InstructionPrototype proto(
        "MOV $1, $2", 0x1C2, 12, {OperandType::Register, OperandType::Literal32}
    );

    EXPECT_EQ(proto.size(), 6);
}

TEST(InstructionPrototype, DoesMatchEightBitOpcode) {
    InstructionPrototype proto(
        "SUB $1, $2", 0x0A, 8, {OperandType::Register, OperandType::Register}
    );

    EXPECT_TRUE(proto.doesMatch({0x0A, 0x12}));
}

TEST(InstructionPrototype, DoesMatchTwelveBitOpcode) {
    InstructionPrototype proto(
        "MOV $1, $2", 0x1B0, 12, {OperandType::Register, OperandType::Address}
    );

    EXPECT_TRUE(proto.doesMatch({0x1B, 0x01, 0x00, 0x00, 0x00, 0x42}));
}

TEST(InstructionPrototype, DoesNotMatchWrongOpcode) {
    InstructionPrototype proto(
        "SUB $1, $2", 0x0A, 8, {OperandType::Register, OperandType::Register}
    );

    EXPECT_FALSE(proto.doesMatch({0x0B, 0x12}));
}

TEST(InstructionPrototype, DoesNotMatchWrongLength) {
    InstructionPrototype proto(
        "SUB $1, $2", 0x0A, 8, {OperandType::Register, OperandType::Register}
    );

    EXPECT_FALSE(proto.doesMatch({0x0A, 0x12, 0x00}));
    EXPECT_FALSE(proto.doesMatch({0x0A}));
}

TEST(InstructionPrototype, DistinguishesTwelveBitNibbles) {
    InstructionPrototype proto1B0(
        "MOV $1, $2", 0x1B0, 12, {OperandType::Register, OperandType::Address}
    );
    InstructionPrototype proto1B1(
        "MOV $2, $1", 0x1B1, 12, {OperandType::Register, OperandType::Address}
    );

    std::vector<std::uint8_t> bytes{0x1B, 0x01, 0x00, 0x00, 0x00, 0x42};

    EXPECT_TRUE(proto1B0.doesMatch(bytes));
    EXPECT_FALSE(proto1B1.doesMatch(bytes));
}

TEST(InstructionPrototype, MatchIgnoresOperandNibble) {
    InstructionPrototype proto(
        "MOV $1, $2", 0x1B0, 12, {OperandType::Register, OperandType::Address}
    );

    EXPECT_TRUE(proto.doesMatch({0x1B, 0x0F, 0x00, 0x00, 0x00, 0x42}));
}

TEST(InstructionPrototype, RegistryContainsNineVariantSixTemplates) {
    EXPECT_EQ(INSTRUCTION_PROTOTYPES.size(), 9);
}

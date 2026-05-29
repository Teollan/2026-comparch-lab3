#include "Operand.hpp"

#include <gtest/gtest.h>

TEST(getOperandBitWidth, AllVariants) {
    EXPECT_EQ(getOperandBitWidth(OperandType::Register), 4);
    EXPECT_EQ(getOperandBitWidth(OperandType::Address), 32);
    EXPECT_EQ(getOperandBitWidth(OperandType::Literal16), 16);
    EXPECT_EQ(getOperandBitWidth(OperandType::Literal32), 32);
}

TEST(formatRegister, LowerNibble) {
    EXPECT_EQ(formatRegister(0x05, true), "R5");
    EXPECT_EQ(formatRegister(0x0F, true), "R15");
    EXPECT_EQ(formatRegister(0x00, true), "R0");
}

TEST(formatRegister, UpperNibble) {
    EXPECT_EQ(formatRegister(0x50, false), "R5");
    EXPECT_EQ(formatRegister(0xF0, false), "R15");
    EXPECT_EQ(formatRegister(0x00, false), "R0");
}

TEST(formatRegister, IgnoresUnrelatedNibble) {
    EXPECT_EQ(formatRegister(0xAB, true), "R11");
    EXPECT_EQ(formatRegister(0xAB, false), "R10");
}

TEST(formatAddress, Zero) {
    EXPECT_EQ(formatAddress({0x00, 0x00, 0x00, 0x00}), "[0x00000000]");
}

TEST(formatAddress, BigEndianAssembly) {
    EXPECT_EQ(formatAddress({0x00, 0xC0, 0x00, 0xA1}), "[0x00C000A1]");
    EXPECT_EQ(formatAddress({0xDE, 0xAD, 0xBE, 0xEF}), "[0xDEADBEEF]");
}

TEST(formatAddress, AllOnes) {
    EXPECT_EQ(formatAddress({0xFF, 0xFF, 0xFF, 0xFF}), "[0xFFFFFFFF]");
}

TEST(formatLiteral16, Zero) {
    EXPECT_EQ(formatLiteral16({0x00, 0x00}), "0");
}

TEST(formatLiteral16, PositiveValues) {
    EXPECT_EQ(formatLiteral16({0x00, 0x01}), "1");
    EXPECT_EQ(formatLiteral16({0x33, 0x44}), "13124");
    EXPECT_EQ(formatLiteral16({0x7F, 0xFF}), "32767");
}

TEST(formatLiteral16, NegativeValues) {
    EXPECT_EQ(formatLiteral16({0xFF, 0xFF}), "-1");
    EXPECT_EQ(formatLiteral16({0x80, 0x00}), "-32768");
    EXPECT_EQ(formatLiteral16({0xFF, 0x80}), "-128");
}

TEST(formatLiteral32, Zero) {
    EXPECT_EQ(formatLiteral32({0x00, 0x00, 0x00, 0x00}), "0");
}

TEST(formatLiteral32, PositiveValues) {
    EXPECT_EQ(formatLiteral32({0x00, 0x00, 0x00, 0x01}), "1");
    EXPECT_EQ(formatLiteral32({0x33, 0x44, 0x55, 0x66}), "860116326");
    EXPECT_EQ(formatLiteral32({0x7F, 0xFF, 0xFF, 0xFF}), "2147483647");
}

TEST(formatLiteral32, NegativeValues) {
    EXPECT_EQ(formatLiteral32({0xFF, 0xFF, 0xFF, 0xFF}), "-1");
    EXPECT_EQ(formatLiteral32({0x80, 0x00, 0x00, 0x00}), "-2147483648");
    EXPECT_EQ(formatLiteral32({0xFF, 0xFF, 0xFF, 0x80}), "-128");
}

TEST(formatOperandBytes, DispatchesToRegister) {
    EXPECT_EQ(formatOperandBytes({0x0A}, OperandType::Register, true), "R10");
    EXPECT_EQ(formatOperandBytes({0xA0}, OperandType::Register, false), "R10");
}

TEST(formatOperandBytes, DispatchesToAddress) {
    EXPECT_EQ(
        formatOperandBytes({0x00, 0x00, 0x00, 0x42}, OperandType::Address),
        "[0x00000042]"
    );
}

TEST(formatOperandBytes, DispatchesToLiteral16) {
    EXPECT_EQ(formatOperandBytes({0xFF, 0xFF}, OperandType::Literal16), "-1");
}

TEST(formatOperandBytes, DispatchesToLiteral32) {
    EXPECT_EQ(
        formatOperandBytes({0x00, 0x00, 0x00, 0x01}, OperandType::Literal32),
        "1"
    );
}

TEST(formatOperandBytes, ThrowsOnUnknownType) {
    auto bogus = static_cast<OperandType>(0xFF);
    EXPECT_THROW(formatOperandBytes({0x00}, bogus), std::invalid_argument);
}

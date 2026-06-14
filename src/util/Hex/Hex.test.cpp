#include "util/Hex/Hex.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

using Bytes = std::vector<std::uint8_t>;

TEST(hexToBytes, Empty) {
    EXPECT_EQ(hexToBytes(""), Bytes{});
}

TEST(hexToBytes, WhitespaceOnly) {
    EXPECT_EQ(hexToBytes("   \t\n  "), Bytes{});
}

TEST(hexToBytes, SingleByte) {
    EXPECT_EQ(hexToBytes("0A"), Bytes{0x0A});
    EXPECT_EQ(hexToBytes("FF"), Bytes{0xFF});
}

TEST(hexToBytes, CaseInsensitive) {
    EXPECT_EQ(hexToBytes("ab"), Bytes{0xAB});
    EXPECT_EQ(hexToBytes("Ab"), Bytes{0xAB});
    EXPECT_EQ(hexToBytes("AB"), Bytes{0xAB});
}

TEST(hexToBytes, SpaceSeparated) {
    EXPECT_EQ(hexToBytes("0A 12 80 AF"), (Bytes{0x0A, 0x12, 0x80, 0xAF}));
}

TEST(hexToBytes, NoSeparators) {
    EXPECT_EQ(hexToBytes("0A1280AF"), (Bytes{0x0A, 0x12, 0x80, 0xAF}));
}

TEST(hexToBytes, MixedWhitespace) {
    EXPECT_EQ(hexToBytes("0A\t12\n80\r\nAF"), (Bytes{0x0A, 0x12, 0x80, 0xAF}));
}

TEST(hexToBytes, OddDigitCountThrows) {
    EXPECT_THROW(hexToBytes("A"), std::invalid_argument);
    EXPECT_THROW(hexToBytes("0A 12 8"), std::invalid_argument);
}

TEST(hexToBytes, NonHexCharsAreSkipped) {
    EXPECT_EQ(hexToBytes("0A, 12; 80 | AF"), (Bytes{0x0A, 0x12, 0x80, 0xAF}));
}

TEST(hexToBytes, LineCommentIsSkipped) {
    EXPECT_EQ(
        hexToBytes("0A 12 // SUB R1, R2\n80 AF // CMP R10, R15\n"),
        (Bytes{0x0A, 0x12, 0x80, 0xAF})
    );
}

TEST(hexToBytes, FullLineComment) {
    EXPECT_EQ(
        hexToBytes("// this is a header comment\n0A 12\n"), (Bytes{0x0A, 0x12})
    );
}

TEST(hexToBytes, CommentSkipsHexLikeChars) {
    EXPECT_EQ(
        hexToBytes("0A 12 // AF BE EF\n80 AF"), (Bytes{0x0A, 0x12, 0x80, 0xAF})
    );
}

TEST(hexToBytes, CommentAtEndOfInputWithoutNewline) {
    EXPECT_EQ(hexToBytes("0A 12 // no trailing newline"), (Bytes{0x0A, 0x12}));
}

TEST(hexToBytes, SingleSlashIsNotAComment) {
    EXPECT_EQ(hexToBytes("0A / 12"), (Bytes{0x0A, 0x12}));
}

TEST(bytesToHex, Empty) {
    EXPECT_EQ(bytesToHex({}), "");
}

TEST(bytesToHex, SingleByte) {
    EXPECT_EQ(bytesToHex({0x0A}), "0A ");
    EXPECT_EQ(bytesToHex({0xFF}), "FF ");
}

TEST(bytesToHex, MultipleBytes) {
    EXPECT_EQ(bytesToHex({0x0A, 0x12, 0x80, 0xAF}), "0A 12 80 AF ");
}

TEST(bytesToHex, ZeroPadding) {
    EXPECT_EQ(bytesToHex({0x00, 0x01, 0x0F}), "00 01 0F ");
}

TEST(bytesToHex, UppercaseHex) {
    EXPECT_EQ(bytesToHex({0xAB, 0xCD, 0xEF}), "AB CD EF ");
}

TEST(Roundtrip, BytesToHexToBytes) {
    Bytes original{0x1B, 0x01, 0x00, 0x00, 0x00, 0x42};
    EXPECT_EQ(hexToBytes(bytesToHex(original)), original);
}

TEST(Roundtrip, HexToBytesToHex) {
    std::string normalized = "1B 01 00 00 00 42 ";
    EXPECT_EQ(bytesToHex(hexToBytes(normalized)), normalized);
}

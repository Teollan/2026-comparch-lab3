#include "assembly/Decompiler/Decompiler.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "vram/PagesTable/PagesTable.hpp"
#include "vram/SegmentsTable/SegmentsTable.hpp"

using Bytes = std::vector<std::uint8_t>;

namespace {

Vram makeIdentityVram() {
    std::vector<PageDescriptor> pages;
    pages.push_back(PageDescriptor{.isResident = true, .frame = 0});

    std::vector<SegmentDescriptor> segments;
    segments.push_back(
        SegmentDescriptor{
            .pagesTable = std::make_unique<PagesTable>(std::move(pages))
        }
    );

    return Vram(std::make_unique<SegmentsTable>(std::move(segments)));
}

}  // namespace

TEST(decompile, EmptyInputReturnsEmptyResult) {
    Vram vram = makeIdentityVram();
    Bytes bytes;
    auto result = decompile(bytes, vram);

    EXPECT_TRUE(result.empty());
}

TEST(decompile, SingleTwoByteInstruction) {
    Vram vram = makeIdentityVram();
    Bytes bytes{0x0A, 0x12};
    auto result = decompile(bytes, vram);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].prototype.opcode, 0x0A);
    EXPECT_EQ(result[0].offset(), 0u);
    EXPECT_EQ(result[0].size(), 2u);
}

TEST(decompile, SingleThreeByteInstruction) {
    Vram vram = makeIdentityVram();
    Bytes bytes{0x03, 0x01, 0x23};
    auto result = decompile(bytes, vram);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].prototype.opcode, 0x030);
}

TEST(decompile, SingleSixByteInstruction) {
    Vram vram = makeIdentityVram();
    Bytes bytes{0x1B, 0x01, 0x00, 0x00, 0x00, 0x42};
    auto result = decompile(bytes, vram);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].prototype.opcode, 0x1B0);
}

TEST(decompile, MultipleInstructionsSameSize) {
    Vram vram = makeIdentityVram();
    Bytes bytes{0x0A, 0x12, 0x80, 0xAF};
    auto result = decompile(bytes, vram);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].prototype.opcode, 0x0A);
    EXPECT_EQ(result[1].prototype.opcode, 0x80);
}

TEST(decompile, OffsetsTrackInstructionStarts) {
    Vram vram = makeIdentityVram();
    Bytes bytes{
        0x0A,
        0x12,  // SUB R1, R2
        0x03,
        0x01,
        0x23,  // ADD R1, R2, R3
        0x1B,
        0x01,
        0x00,
        0x00,
        0x00,
        0x42,  // MOV R1, [...]
        0x80,
        0xAF,  // CMP R10, R15
    };
    auto result = decompile(bytes, vram);

    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0].offset(), 0u);
    EXPECT_EQ(result[1].offset(), 2u);
    EXPECT_EQ(result[2].offset(), 5u);
    EXPECT_EQ(result[3].offset(), 11u);
}

TEST(decompile, DistinguishesNibbleVariantOpcodes) {
    Vram vram = makeIdentityVram();
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
    auto result = decompile(bytes, vram);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].prototype.opcode, 0x1B0);
    EXPECT_EQ(result[1].prototype.opcode, 0x1B1);
}

TEST(decompile, DistinguishesLit16AndLit32) {
    Vram vram = makeIdentityVram();
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
    auto result = decompile(bytes, vram);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].prototype.opcode, 0x1C1);
    EXPECT_EQ(result[0].size(), 4u);
    EXPECT_EQ(result[1].prototype.opcode, 0x1C2);
    EXPECT_EQ(result[1].size(), 6u);
}

TEST(decompile, ThrowsOnUnknownOpcode) {
    Vram vram = makeIdentityVram();
    Bytes bytes{0xFF, 0x00};

    EXPECT_THROW(decompile(bytes, vram), DecompilerUnknownInstructionException);
}

TEST(decompile, ThrowsAfterDecodingValidPrefix) {
    Vram vram = makeIdentityVram();
    Bytes bytes{0x0A, 0x12, 0xFF, 0x00};

    EXPECT_THROW(decompile(bytes, vram), DecompilerUnknownInstructionException);
}

TEST(decompile, PropagatesTranslationFailure) {
    Vram vram = makeIdentityVram();
    Bytes bytes{0x1B, 0x01, 0x00, 0x50, 0x00, 0x42};
    auto result = decompile(bytes, vram);

    ASSERT_EQ(result.size(), 1);
    ASSERT_TRUE(result[0].error().has_value());
    EXPECT_EQ(result[0].error()->error, TranslationError::SegmentNotFound);
}

#include "assembly/Instruction/Instruction.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "vram/PagesTable/PagesTable.hpp"
#include "vram/SegmentsTable/SegmentsTable.hpp"

using Bytes = std::vector<std::uint8_t>;

namespace {

Vram makeVram(std::uint32_t frame) {
    std::vector<PageDescriptor> pages;
    pages.push_back(PageDescriptor{.isResident = true, .frame = frame});

    std::vector<SegmentDescriptor> segments;
    segments.push_back(
        SegmentDescriptor{
            .pagesTable = std::make_unique<PagesTable>(std::move(pages))
        }
    );

    return Vram(std::make_unique<SegmentsTable>(std::move(segments)));
}

}  // namespace

TEST(Instruction, StoresPrototypeOffsetAndSize) {
    Vram vram = makeVram(0);
    InstructionPrototype proto(
        "SUB $1, $2", 0x0A, 8, {OperandType::Register, OperandType::Register}
    );
    Instruction instr(proto, {0x0A, 0x12}, 7, vram);

    EXPECT_EQ(&instr.prototype, &proto);
    EXPECT_EQ(instr.offset(), 7u);
    EXPECT_EQ(instr.size(), 2u);
}

TEST(Instruction, GetOperandBytesIsolatesRegisterNibble) {
    Vram vram = makeVram(0);
    InstructionPrototype proto(
        "SUB $1, $2", 0x0A, 8, {OperandType::Register, OperandType::Register}
    );
    Instruction instr(proto, {0x0A, 0x12}, 0, vram);

    EXPECT_FALSE(instr.error().has_value());
    EXPECT_EQ(instr.getOperandBytes(0), Bytes{0x01});
    EXPECT_EQ(instr.getOperandBytes(1), Bytes{0x02});
}

TEST(Instruction, TranslatesAddressOperandToPhysical) {
    Vram vram = makeVram(0x10);
    InstructionPrototype proto(
        "MOV $1, $2", 0x1B0, 12, {OperandType::Register, OperandType::Address}
    );
    // seg 0 / page 0 / off 0x42, frame 0x10 -> (0x10 << 10) | 0x42 = 0x4042
    Instruction instr(proto, {0x1B, 0x01, 0x00, 0x00, 0x00, 0x42}, 0, vram);

    EXPECT_FALSE(instr.error().has_value());
    EXPECT_EQ(instr.getOperandBytes(1), (Bytes{0x00, 0x00, 0x40, 0x42}));
}

TEST(Instruction, RecordsErrorAndKeepsVirtualAddressOnFailure) {
    Vram vram = makeVram(0);
    InstructionPrototype proto(
        "MOV $1, $2", 0x1B0, 12, {OperandType::Register, OperandType::Address}
    );
    Instruction instr(proto, {0x1B, 0x01, 0x00, 0x50, 0x00, 0x42}, 0, vram);

    ASSERT_TRUE(instr.error().has_value());
    EXPECT_EQ(instr.error()->error, TranslationError::SegmentNotFound);
    EXPECT_EQ(instr.error()->virtualAddress, 0x00500042u);
    EXPECT_EQ(instr.getOperandBytes(1), (Bytes{0x00, 0x50, 0x00, 0x42}));
}

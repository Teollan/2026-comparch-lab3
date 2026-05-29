#include "Instruction.hpp"

#include <gtest/gtest.h>

using Bytes = std::vector<std::uint8_t>;

TEST(Instruction, ConstructorStoresPrototypeAndBytes) {
    InstructionPrototype proto(
        "SUB $1, $2", 0x0A, 8, {OperandType::Register, OperandType::Register}
    );
    Instruction instr(proto, {0x0A, 0x12});

    EXPECT_EQ(&instr.prototype, &proto);
    EXPECT_EQ(instr.bytes, (Bytes{0x0A, 0x12}));
}

TEST(Instruction, ToAsmTwoRegsEightBitOpcode) {
    InstructionPrototype proto(
        "SUB $1, $2", 0x0A, 8, {OperandType::Register, OperandType::Register}
    );
    Instruction instr(proto, {0x0A, 0x12});

    EXPECT_EQ(instr.toAsm(), "SUB R1, R2");
}

TEST(Instruction, ToAsmHighAndLowNibbleRegs) {
    InstructionPrototype proto(
        "CMP $1, $2", 0x80, 8, {OperandType::Register, OperandType::Register}
    );
    Instruction instr(proto, {0x80, 0xAF});

    EXPECT_EQ(instr.toAsm(), "CMP R10, R15");
}

TEST(Instruction, ToAsmRegAddrTwelveBitOpcode) {
    InstructionPrototype proto(
        "MOV $1, $2", 0x1B0, 12, {OperandType::Register, OperandType::Address}
    );
    Instruction instr(proto, {0x1B, 0x01, 0x00, 0x00, 0x00, 0x42});

    EXPECT_EQ(instr.toAsm(), "MOV R1, [0x00000042]");
}

TEST(Instruction, ToAsmSwappedOperandOrderInFormat) {
    InstructionPrototype proto(
        "MOV $2, $1", 0x1B1, 12, {OperandType::Register, OperandType::Address}
    );
    Instruction instr(proto, {0x1B, 0x12, 0x00, 0x00, 0x00, 0x42});

    EXPECT_EQ(instr.toAsm(), "MOV [0x00000042], R2");
}

TEST(Instruction, ToAsmThreeRegs) {
    InstructionPrototype proto(
        "ADD $1, $2, $3",
        0x030,
        12,
        {OperandType::Register, OperandType::Register, OperandType::Register}
    );
    Instruction instr(proto, {0x03, 0x01, 0x23});

    EXPECT_EQ(instr.toAsm(), "ADD R1, R2, R3");
}

TEST(Instruction, ToAsmLiteral16Positive) {
    InstructionPrototype proto(
        "MOV $1, $2", 0x1C1, 12, {OperandType::Register, OperandType::Literal16}
    );
    Instruction instr(proto, {0x1C, 0x11, 0x33, 0x44});

    EXPECT_EQ(instr.toAsm(), "MOV R1, 13124");
}

TEST(Instruction, ToAsmLiteral16Negative) {
    InstructionPrototype proto(
        "MOV $1, $2", 0x1C1, 12, {OperandType::Register, OperandType::Literal16}
    );
    Instruction instr(proto, {0x1C, 0x1F, 0xFF, 0xFF});

    EXPECT_EQ(instr.toAsm(), "MOV R15, -1");
}

TEST(Instruction, ToAsmLiteral32Positive) {
    InstructionPrototype proto(
        "MOV $1, $2", 0x1C2, 12, {OperandType::Register, OperandType::Literal32}
    );
    Instruction instr(proto, {0x1C, 0x21, 0x33, 0x44, 0x55, 0x66});

    EXPECT_EQ(instr.toAsm(), "MOV R1, 860116326");
}

TEST(Instruction, ToAsmLiteral32Negative) {
    InstructionPrototype proto(
        "MOV $1, $2", 0x1C2, 12, {OperandType::Register, OperandType::Literal32}
    );
    Instruction instr(proto, {0x1C, 0x2F, 0xFF, 0xFF, 0xFF, 0xFF});

    EXPECT_EQ(instr.toAsm(), "MOV R15, -1");
}

TEST(Instruction, GetOperandBytesRegsShareSameByte) {
    // 8-bit opcode + two 4-bit regs: both operands live in byte 1
    InstructionPrototype proto(
        "SUB $1, $2", 0x0A, 8, {OperandType::Register, OperandType::Register}
    );
    Instruction instr(proto, {0x0A, 0x12});

    EXPECT_EQ(instr.getOperandBytes(0), Bytes{0x12});
    EXPECT_EQ(instr.getOperandBytes(1), Bytes{0x12});
}

TEST(Instruction, GetOperandBytesRegThenAddress) {
    InstructionPrototype proto(
        "MOV $1, $2", 0x1B0, 12, {OperandType::Register, OperandType::Address}
    );
    Instruction instr(proto, {0x1B, 0x01, 0x00, 0x00, 0x00, 0x42});

    EXPECT_EQ(instr.getOperandBytes(0), Bytes{0x01});
    EXPECT_EQ(instr.getOperandBytes(1), (Bytes{0x00, 0x00, 0x00, 0x42}));
}

#include "chip8.h"

#include "iopparser.h"
#include "defs.h"
#include "ParsedOpResults.h"

#include <tuple>
#include <optional>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

struct opParserMock : IOpParser
{
    MOCK_METHOD(ParsedOpResults, parseOp, (const uint16_t), (const override));
};

TEST(chip8, ctor) {
    opParserMock mock{};
    Chip8 chip8{mock};
}

TEST(chip8, clearScreen) {
    opParserMock mock{};
    Chip8 chip8{mock};

    ParsedOpResults returnVal {Op::CLEAR_SCREEN};
    EXPECT_CALL(mock, parseOp(0x00E0)).Times(1).WillOnce(::testing::Return(returnVal));
    chip8.parseOp(0x00E0);
}

TEST(chip8, jump) {
    opParserMock mock{};
    Chip8 chip8{mock};

    ParsedOpResults returnVal = {Op::JUMP, 0x0111};
    EXPECT_CALL(mock, parseOp(0x1111)).Times(1).WillOnce(::testing::Return(returnVal));
    chip8.parseOp(0x1111);
    EXPECT_EQ(chip8.getPc(), 0x0111);
}

TEST(chip8, SetRegister) {
    opParserMock mock{};
    Chip8 chip8{mock};

    RegValue regVal {0x0001, 0x0011};
    ParsedOpResults returnVal = {Op::SET_REGISTER, regVal};
    auto op = 0x6111;
    EXPECT_CALL(mock, parseOp(op)).Times(1).WillOnce(::testing::Return(returnVal));
    chip8.parseOp(op);
    EXPECT_EQ(chip8.getRegisters()[1], 17);
}
#include "chip8.h"

#include "iopparser.h"
#include "IDrawer.h"
#include "defs.h"
#include "ParsedOpResults.h"
#include "opparser.h"

#include <memory>
#include <tuple>
#include <optional>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

struct opParserMock : IOpParser
{
    MOCK_METHOD(ParsedOpResults, parseOp, (const uint16_t), (const override));
};

struct DrawerMock : IDrawer
{
    MOCK_METHOD(void, draw, (), (const override));
};

class Chip8Test : public ::testing::Test
{
    protected:

    opParserMock mock{};
    DrawerMock drawer{};
    Chip8 chip8 {mock, drawer};

    void setRegister(Chip8& chip8, const opParserMock& mock, const uint16_t expectedRegVal)
    {
        RegValue regVal {0x0001, 0x0011};
        ParsedOpResults returnVal = {Op::SET_REGISTER, regVal};
        auto op = 0x6111;
        EXPECT_CALL(mock, parseOp(op)).Times(1).WillOnce(::testing::Return(returnVal));
        chip8.parseOp(op);
        EXPECT_EQ(chip8.getRegisters()[1], expectedRegVal);
    }
};

TEST_F(Chip8Test, clearScreen) {

    ParsedOpResults returnVal {Op::CLEAR_SCREEN};
    EXPECT_CALL(mock, parseOp(0x00E0)).Times(1).WillOnce(::testing::Return(returnVal));
    chip8.parseOp(0x00E0);
}

TEST_F(Chip8Test, jump) {
    ParsedOpResults returnVal = {Op::JUMP, 0x0111};
    EXPECT_CALL(mock, parseOp(0x1111)).Times(1).WillOnce(::testing::Return(returnVal));
    chip8.parseOp(0x1111);
    EXPECT_EQ(chip8.getPc(), 0x0111);
}


TEST_F(Chip8Test, SetRegister) {
    setRegister(chip8, mock, 17);
}

TEST_F(Chip8Test, AddRegister) {
    // Arrage
    constexpr uint16_t expectedRegValue = 17;
    setRegister(chip8, mock, expectedRegValue);

    RegValue regVal {0x0001, 0x001};
    ParsedOpResults returnVal = {Op::ADD_REGISTER, regVal};
    auto op = 0x7101;
    EXPECT_CALL(mock, parseOp(op)).Times(1).WillOnce(::testing::Return(returnVal));
    chip8.parseOp(op);
    EXPECT_EQ(chip8.getRegisters()[1], expectedRegValue+1);
}

TEST_F(Chip8Test, setIndexRegister) {
    // Arrage
    RegValue regVal {0x0, 0x0234};
    ParsedOpResults returnVal = {Op::SET_INDEX_REGISTER, regVal};
    auto op = 0xA234;
    EXPECT_CALL(mock, parseOp(op)).Times(1).WillOnce(::testing::Return(returnVal));
    chip8.parseOp(op);
    EXPECT_EQ(chip8.getIndexRegister(), 0x0234);

    // Act & Assert
}

TEST_F(Chip8Test, draw) {
    // Arrage
    auto op = 0xD234;
    ParsedOpResults returnVal {Op::DRAW, 0x0234};
    EXPECT_CALL(mock, parseOp(op)).Times(1).WillOnce(::testing::Return(returnVal));
    EXPECT_CALL(drawer, draw).Times(1);
    chip8.parseOp(op);

    // Act & Assert
}

TEST_F(Chip8Test, drawV2)
{
    OpParser parser{};
    DrawerMock drawer{};
    Chip8 newChip8 = {parser, drawer};

    std::array<uint16_t, 68> opCodes{0x00e0,0xa22a, 0x600c, 0x6108, 0xd01f, 0x7009,0xa239, 0xd01f,
    0xa248, 0x7008, 0xd01f, 0x7004, 0xa257, 0xd01f, 0x7008, 0xa266,  
    0xd01f, 0x7008, 0xa275, 0xd01f, 0x1228, 0xff00, 0xff00, 0x3c00,  
    0x3c00, 0x3c00, 0x3c00, 0xff00, 0xffff, 0x00ff, 0x0038, 0x003f,  
    0x003f, 0x0038, 0x00ff, 0x00ff, 0x8000, 0xe000, 0xe000, 0x8000,  
    0x8000, 0xe000, 0xe000, 0x80f8, 0x00fc, 0x003e, 0x003f, 0x003b,  
    0x0039, 0x00f8, 0x00f8, 0x0300, 0x0700, 0x0f00, 0xbf00, 0xfb00,  
    0xf300, 0xe300, 0x43e0, 0x00e0, 0x0080, 0x0080, 0x0080, 0x0080,
    0x00e0, 0x00e0 };
    for(const auto& op : opCodes)
    {
        newChip8.parseOp(op);
    }
}
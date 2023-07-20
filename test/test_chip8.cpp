#include "chip8.h"

#include "IDrawer.h"
#include "defs.h"
#include "ParsedOpResults.h"
#include "opparser.h"

#include <memory>
#include <tuple>
#include <optional>
#include <array>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

struct opParserMock : OpParser
{
    MOCK_METHOD(ParsedOpResults, parseOp, (const uint16_t), (const override));
};

struct DrawerMock : IDrawer
{
    using pixelInput = const std::array<std::array<uint16_t, 64>, 32>&;
    MOCK_METHOD(void, draw, (pixelInput), (const override));
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

TEST_F(Chip8Test, DISABLED_drawV2)
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

TEST_F(Chip8Test, loadProgram)
{
    std::array<uint8_t, 2> program{0xA2, 0x34};
    RegValue regVal {0x0, 0x0234};
    ParsedOpResults returnVal = {Op::SET_INDEX_REGISTER, regVal};
    auto op = 0xA234;
    EXPECT_CALL(mock, parseOp(op)).Times(1).WillOnce(::testing::Return(returnVal));
    EXPECT_CALL(drawer, draw).Times(0);
    chip8.load_program(program);

    chip8.run();

    EXPECT_EQ(chip8.getIndexRegister(), 0x0234);
}

// 00e0, 0x6101, 0x6008, 0xa250, 0xd01f, 0x6010, 0xa25f, 0xd01f
// 6018 a26e d01f 6020 a27d d01f 6028 a28c
// d01f 6030 a29b d01f 6110 6008 a2aa d01f
// 6010 a2b9 d01f 6018 a2c8 d01f 6020 a2d7
// d01f 6028 a2e6 d01f 6030 a2f5 d01f 124e
// 0f02 0202 0202 0000 1f3f 71e0 e5e0 e8a0
// 0d2a 2828 2800 0018 b8b8 3838 3fbf 00 19
// a5 bd a1 9d 00 00 0c 1d 1d 01 0d 1d 9d 01 c7 29
// 29 29 27 00 00 f8 fc ce c6 c6 c6 c6 00 49 4a 49
// 48 3b 00 00 00 01 03 03 03 01 f0 30 90 00 00 80
// 00 00 00 fe c7 83 83 83 c6 fc e7 e0 e0 e0 e0 71
// 3f 1f 00 00 07 02 02 02 02 39 38 38 38 38 b8 b8
// 38 00 00 31 4a 79 40 3b dd dd dd dd dd dd dd dd
// 00 00 a0 38 20 a0 18 ce fc f8 c0 d4 dc c4 c5 00
// 00 30 44 24 14 63 f1 03 07 07 77 57 53 71 00 00
// 28 8e a8 a8 a6 ce 87 03 03 03 87 fe fc 00 00 60
// 90 f0 80 70 

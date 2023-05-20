#include "chip8.h"

#include "iopparser.h"
#include "IDrawer.h"
#include "defs.h"
#include "ParsedOpResults.h"

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
    std::unique_ptr<Chip8> chip8;
    virtual void SetUp()
    {
        chip8 = std::make_unique<Chip8>(mock, drawer);
    }

    void setRegister(std::unique_ptr<Chip8>& chip8, const opParserMock& mock, const uint16_t expectedRegVal)
    {
        RegValue regVal {0x0001, 0x0011};
        ParsedOpResults returnVal = {Op::SET_REGISTER, regVal};
        auto op = 0x6111;
        EXPECT_CALL(mock, parseOp(op)).Times(1).WillOnce(::testing::Return(returnVal));
        chip8->parseOp(op);
        EXPECT_EQ(chip8->getRegisters()[1], expectedRegVal);
    }
};

TEST_F(Chip8Test, clearScreen) {

    ParsedOpResults returnVal {Op::CLEAR_SCREEN};
    EXPECT_CALL(mock, parseOp(0x00E0)).Times(1).WillOnce(::testing::Return(returnVal));
    chip8->parseOp(0x00E0);
}

TEST_F(Chip8Test, jump) {
    ParsedOpResults returnVal = {Op::JUMP, 0x0111};
    EXPECT_CALL(mock, parseOp(0x1111)).Times(1).WillOnce(::testing::Return(returnVal));
    chip8->parseOp(0x1111);
    EXPECT_EQ(chip8->getPc(), 0x0111);
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
    chip8->parseOp(op);
    EXPECT_EQ(chip8->getRegisters()[1], expectedRegValue+1);
}

TEST_F(Chip8Test, setIndexRegister) {
    // Arrage
    RegValue regVal {0x0, 0x0234};
    ParsedOpResults returnVal = {Op::SET_INDEX_REGISTER, regVal};
    auto op = 0xA234;
    EXPECT_CALL(mock, parseOp(op)).Times(1).WillOnce(::testing::Return(returnVal));
    chip8->parseOp(op);
    EXPECT_EQ(chip8->getIndexRegister(), 0x0234);

    // Act & Assert
}

TEST_F(Chip8Test, draw) {
    // Arrage
    auto op = 0xD234;
    ParsedOpResults returnVal = {Op::DRAW, 0x0};
    EXPECT_CALL(mock, parseOp(op)).Times(1).WillOnce(::testing::Return(returnVal));
    EXPECT_CALL(drawer, draw).Times(1);
    chip8->parseOp(op);

    // Act & Assert
}
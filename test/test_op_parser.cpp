#include "opparser.h"
#include "defs.h"
#include "ParsedOpResults.h"

#include <cstdint>
#include <tuple>
#include "gtest/gtest.h"

TEST(opParser, clearScreen) {
    constexpr OpParser cut;

    constexpr ParsedOpResults expected {Op::CLEAR_SCREEN};
    auto result = cut.parseOp(0x00E0);
    EXPECT_EQ(result, expected);
}

TEST(opParser, jump) {
    OpParser cut;

    const ParsedOpResults expected {Op::JUMP, 0x234};
    EXPECT_EQ(cut.parseOp(0x1234), expected);
}

TEST(opParser, setRegisters) {
    OpParser cut;

    constexpr RegValue regVal{0x0002, 0x0034};
    const ParsedOpResults expected {Op::SET_REGISTER, regVal};
    EXPECT_EQ(cut.parseOp(0x6234), expected);
}

TEST(opParser, addRegisters) {
    // Arrage
    OpParser cut;

    constexpr RegValue regVal{0x0002, 0x0034};
    const ParsedOpResults expected {Op::SET_REGISTER, regVal};
    EXPECT_EQ(cut.parseOp(0x6234), expected);

    // Act & Assert
    constexpr RegValue regVal2{0x0002, 0x0001};
    const ParsedOpResults expected2 {Op::ADD_REGISTER, regVal2};
    EXPECT_EQ(cut.parseOp(0x7201), expected2);
}

TEST(opParser, setIndexRegister) {
    OpParser cut;

    constexpr RegValue regVal{0x0, 0x0234};
    const ParsedOpResults expected {Op::SET_INDEX_REGISTER, regVal};
    EXPECT_EQ(cut.parseOp(0xA234), expected);
}

TEST(opParser, draw) {
    OpParser cut;

    constexpr RegValue regVal{};
    const ParsedOpResults expected {Op::DRAW, 0x0234};
    EXPECT_EQ(cut.parseOp(0xD234), expected);
}
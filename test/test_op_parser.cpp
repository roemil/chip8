#include "opparser.h"
#include "defs.h"

#include <cstdint>
#include <tuple>
#include "gtest/gtest.h"

TEST(opParser, clearScreen) {
    OpParser cut;

    constexpr auto expected = std::tuple<Op, uint16_t, uint16_t>{Op::CLEAR_SCREEN, 0x0, 0x0};
    EXPECT_EQ(cut.parseOp(0x00E0), expected);
}

TEST(opParser, jump) {
    OpParser cut;

    constexpr auto expected = std::tuple<Op, uint16_t, uint16_t>{Op::JUMP, 0x234, 0x0};
    EXPECT_EQ(cut.parseOp(0x1234), expected);
}

TEST(opParser, setRegisters) {
    OpParser cut;

    constexpr auto expected = std::tuple<Op, uint16_t, uint16_t>{Op::SET_REGISTER, 0x0002, 0x0034};
    EXPECT_EQ(cut.parseOp(0x6234), expected);
}
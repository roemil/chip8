#include "opparser.h"
#include "defs.h"
#include "ParsedOpResults.h"

#include <cstdint>
#include <tuple>
#include "gtest/gtest.h"

// TEST(opParser, clearScreen) {
//     OpParser cut;

//     const ParsedOpResults expected {Op::CLEAR_SCREEN};
//     EXPECT_EQ(cut.parseOp(0x00E0), expected);
// }

// TEST(opParser, jump) {
//     OpParser cut;

//     const ParsedOpResults expected {Op::JUMP, 0x234};
//     EXPECT_EQ(cut.parseOp(0x1234), expected);
// }

// TEST(opParser, setRegisters) {
//     OpParser cut;

//     const RegValue regVal{0x0002, 0x0034};
//     const ParsedOpResults expected {Op::JUMP, regVal};
//     EXPECT_EQ(cut.parseOp(0x6234), expected);
// }
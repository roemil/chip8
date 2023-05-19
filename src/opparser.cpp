#include "opparser.h"

#include "defs.h"
#include <stdexcept>
#include <tuple>

std::tuple<Op, uint16_t, uint16_t> OpParser::parseOp(const uint16_t op) const
{
    switch(op & 0xF000)
    {
        case 0x0:
            return {Op::CLEAR_SCREEN, 0x0, 0x0};
        case 0x1000:
        {
            return {Op::JUMP, op & 0x0FFF, 0x0};
        }
        case 0x6000:
            return {Op::SET_REGISTER, (op & 0x0F00) >> 8, op & 0x00FF};
        case 0x7000:
            return {Op::ADD_REGISTER, (op & 0x0F00) >> 8, op & 0x00FF};
        case 0xA000:
            return {Op::SET_INDEX_REGISTER, op & 0x0FFF, 0x0};
        case 0xD000:
            return {Op::DRAW, 0x0, 0x0};
        default:
            throw std::invalid_argument{"Bad op"};
    }
}
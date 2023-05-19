#include "opparser.h"

#include "ParsedOpResults.h"
#include "RegValue.h"

#include "defs.h"
#include <stdexcept>
#include <tuple>

ParsedOpResults OpParser::parseOp(const uint16_t op) const
{
    switch(op & 0xF000)
    {
        case 0x0:
            return ParsedOpResults{Op::CLEAR_SCREEN};
        case 0x1000:
        {
            return ParsedOpResults{Op::JUMP, static_cast<uint16_t>(op & 0x0FFF)};
        }
        case 0x6000:
            return ParsedOpResults{Op::SET_REGISTER, RegValue((op & 0x0F00) >> 8, op & 0x00FF)};
        case 0x7000:
            return ParsedOpResults{Op::ADD_REGISTER, RegValue((op & 0x0F00) >> 8, op & 0x00FF)};
        case 0xA000:
            return ParsedOpResults{Op::SET_INDEX_REGISTER, static_cast<uint16_t>(op & 0x0FFF)};
        case 0xD000:
            return ParsedOpResults{Op::DRAW};
        default:
            throw std::invalid_argument{"Bad op"};
    }
}
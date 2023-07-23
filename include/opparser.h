#pragma once

#include "ParsedOpResults.h"

#include <cstdint>
#include <string>
#include <iostream>

enum class Op;

class OpParser
{
public:
    constexpr ~OpParser() = default;
    constexpr OpParser() = default;

    [[nodiscard]] virtual ParsedOpResults parseOp(const uint16_t op) const
    {
        std::cout << "Parsing op " << op << std::endl;
        switch(op & 0xF000)
        {
            case 0x0:
                return ParsedOpResults{Op::CLEAR_SCREEN};
            case 0x1000:
                return ParsedOpResults{Op::JUMP, static_cast<uint16_t>(op & 0x0FFF)};
            case 0x6000:
                return ParsedOpResults{Op::SET_REGISTER, RegValue((op & 0x0F00) >> 8, op & 0x00FF)};
            case 0x7000:
                return ParsedOpResults{Op::ADD_REGISTER, RegValue((op & 0x0F00) >> 8, op & 0x00FF)};
            case 0xA000:
                return ParsedOpResults{Op::SET_INDEX_REGISTER, RegValue(0x0, op & 0x0FFF)};
            case 0xD000:
                return ParsedOpResults{Op::DRAW, static_cast<uint16_t>(op & 0x0FFF)};
            default:
                //std::stringstream ss;
                //ss << std::hex << op;
                throw std::invalid_argument{"Bad op: " + std::to_string(op)};
        }
    }
};
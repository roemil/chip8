#include "chip8.h"

#include "iopparser.h"
#include "IDrawer.h"
#include "defs.h"
#include "ParsedOpResults.h"
#include "RegValue.h"

#include <iostream>
#include <stdexcept>
#include <tuple>
constexpr void Chip8::setFontSprite()
{

/*    Font sprite 050-09F
0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80  // F
*/

constexpr std::array<uint8_t, 80> fontSprits {0xF0, 0x90, 0x90, 0x90, 0xF0,
                                 0x20, 0x60, 0x20, 0x20, 0x70,
                                 0xF0, 0x10, 0xF0, 0x80, 0xF0,
                                 0xF0, 0x10, 0xF0, 0x10, 0xF0,
                                 0x90, 0x90, 0xF0, 0x10, 0x10,
                                 0xF0, 0x80, 0xF0, 0x10, 0xF0,
                                 0xF0, 0x80, 0xF0, 0x90, 0xF0,
                                 0xF0, 0x10, 0x20, 0x40, 0x40,
                                 0xF0, 0x90, 0xF0, 0x90, 0xF0,
                                 0xF0, 0x90, 0xF0, 0x10, 0xF0,
                                 0xF0, 0x90, 0xF0, 0x90, 0x90,
                                 0xE0, 0x90, 0xE0, 0x90, 0xE0,
                                 0xF0, 0x80, 0x80, 0x80, 0xF0,
                                 0xE0, 0x90, 0x90, 0x90, 0xE0,
                                 0xF0, 0x80, 0xF0, 0x80, 0xF0, // row 14
                                 0xF0, 0x80, 0xF0, 0x80, 0x80
                                };
    unsigned memLoc = 0x050;
    for(const auto& sprite : fontSprits)
    {
        memory_[memLoc++] = sprite;
    }

}

Chip8::Chip8(const IOpParser& opParser, const IDrawer& drawer) : opParser_{opParser}, drawer_{drawer}
{
    setFontSprite();
}

 void Chip8::clearScreen() const
{
    std::cout << "\nclearScreen\n";
}

void Chip8::jump(const uint16_t newAddress)
{
    pc = newAddress;
}

void Chip8::setRegister(const uint16_t reg, const uint16_t value)
{
    registers_[reg] = value;
}

void Chip8::addToRegister(const uint16_t reg, const uint16_t value)
{
    registers_[reg] += value;
}

void Chip8::parseOp(const uint16_t op)
{
    auto parsedOp = opParser_.parseOp(op);
    switch (parsedOp.op)
    {   case Op::JUMP:
            jump(*parsedOp.address);
            break;
        case Op::SET_REGISTER:
            setRegister(parsedOp.regValue->reg, parsedOp.regValue->value);
            break;
        case Op::ADD_REGISTER:
            addToRegister(parsedOp.regValue->reg, parsedOp.regValue->value);
            break;
        case Op::SET_INDEX_REGISTER:
            indexRegister_ = parsedOp.regValue->value;
        case Op::DRAW:
            drawer_.draw();
            break;
        default:
            // nothing to do here
            break;
    }
}
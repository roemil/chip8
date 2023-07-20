#include "chip8.h"

#include "opparser.h"
#include "IDrawer.h"
#include "defs.h"
#include "ParsedOpResults.h"
#include "RegValue.h"

#include <iostream>
#include <stdexcept>
#include <memory>
#include <string>
#include <utility>
#include <variant>

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
                                              0xF0, 0x80, 0xF0, 0x80, 0xF0,
                                              0xF0, 0x80, 0xF0, 0x80, 0x80
                                             };
    unsigned memLoc = 0x050;
    indexRegister_ = memLoc;
    for(const auto& sprite : fontSprits)
    {
        memory_[memLoc++] = sprite;
    }

}

Chip8::Chip8(const OpParser& opParser, const IDrawer& drawer) : opParser_{opParser}, drawer_{drawer}
{
    pc = program_memory_start;
    setFontSprite();
}

void Chip8::clearScreen()
{
    for(auto& pixelRow : pixels)
    {
        pixelRow.fill(0);
    }
}

void Chip8::jump(const size_type newAddress)
{
    pc = newAddress;
}

void Chip8::setRegister(const size_type reg, const size_type value)
{
    registers_[reg] = value;
}

void Chip8::addToRegister(const uint16_t reg, const uint16_t value)
{
    registers_[reg] += value;
}

void Chip8::draw(const uint16_t drawInstructions)
{
    const uint8_t regX = (drawInstructions & 0x0F00) >> 8;
    const uint8_t xCoord = registers_[regX] % 64;
    const uint8_t regY = (drawInstructions & 0x00F0) >> 4;
    const uint8_t yCoord = registers_[regY] % 32;
    const uint8_t numPixels = drawInstructions & 0x000F;
    vf_ = false;
    const auto oldPixels = pixels;
    for(uint8_t row = 0; row < numPixels; ++row)
    {
            const auto& spritePixel = memory_[indexRegister_];
            for(uint8_t j = 0; j < 8; ++j)
            {
                auto& currentOldScreenPixel = oldPixels[j][row];
                bool isSpritePixelOn = spritePixel & (0x80 >> j);
                if(isSpritePixelOn && currentOldScreenPixel == 1)
                {
                    // turn off pixel
                    pixels[j+xCoord][row+yCoord] = 0;
                    vf_ = true; // is this a list?
                }
                else if(isSpritePixelOn && currentOldScreenPixel == 0)
                {
                    // turn on pixel
                    pixels[j+xCoord][row+yCoord] = 1;
                }
            }
    }

    drawer_.draw(pixels);
}

void Chip8::parseOp(const uint16_t op)
{
    auto parsedOp = opParser_.parseOp(op);
    const auto parseResults = parsedOp.parseResult_;
    switch (parsedOp.op)
    {   case Op::JUMP:
            jump(std::get<uint16_t>(parseResults));
            break;
        case Op::SET_REGISTER:
            setRegister(std::get<RegValue>(parseResults).reg, std::get<RegValue>(parseResults).value);
            break;
        case Op::ADD_REGISTER:
            addToRegister(std::get<RegValue>(parseResults).reg, std::get<RegValue>(parseResults).value);
            break;
        case Op::SET_INDEX_REGISTER:
            indexRegister_ = std::get<RegValue>(parseResults).value;
            break;
        case Op::DRAW:
            draw(std::get<uint16_t>(parseResults));
            break;
        default:
            // nothing to do here
            break;
    }
}

constexpr uint16_t Chip8::nextInstruction() const
{
    uint16_t op = memory_[pc] << 8;
    op |= memory_[pc+1];
    return op;
}

void Chip8::run()
{
    const auto op = nextInstruction();
    parseOp(op);
    pc += 2;
    return;
}
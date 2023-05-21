#include "chip8.h"

#include "iopparser.h"
#include "IDrawer.h"
#include "defs.h"
#include "ParsedOpResults.h"
#include "RegValue.h"
#include "SDL.h"

#include <iostream>
#include <stdexcept>
#include <memory>
#include <string>
#include <utility>

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
    for(const auto& sprite : fontSprits)
    {
        memory_[memLoc++] = sprite;
    }

}

namespace
{
    struct WindowDestroyer
    {
        void operator()(SDL_Window* w) const
        {
            SDL_DestroyWindow(w);
        }
    };
}

Chip8::~Chip8()
{
    SDL_Quit();
}

Chip8::Chip8(const IOpParser& opParser, const IDrawer& drawer) : opParser_{opParser}, drawer_{drawer}
{
    setFontSprite();

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        throw std::runtime_error{"SDL could not initialize! SDL_Error: " + std::string{SDL_GetError()} + "\n"};
    }
    int SCREEN_WIDTH = 640;
    int SCREEN_HEIGHT = 320;
    //Create window
    window.reset(SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    if(!window)
    {
        throw std::runtime_error{"Window could not be created! SDL_Error: " + std::string{SDL_GetError()} + "\n"};
    }

    // Screen surface is owned by window and should not be freed
    screenSurface = SDL_GetWindowSurface(window.get());
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
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;
    int scale = 10;
    rect.h = 64*scale;
    rect.w = 64*scale;
    
    for (int i=0; i<64; i++)
    {
        for (int j=0; j<32; j++)
        {
            rect.x = i*scale;
            rect.y = j*scale;
            rect.w = scale;
            rect.h = scale;
            if (pixels[i][j] == 1)
            {
                SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 255, 255));
            }
            else if (pixels[i][j] == 0)
            {
                SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 0));
            }
        }
    }
    //SDL_Flip(screenSurface);
    //Update the surface
    SDL_UpdateWindowSurface( window.get() );
    drawer_.draw();
}

void Chip8::parseOp(const uint16_t op)
{
    auto parsedOp = opParser_.parseOp(op);
    switch (parsedOp.op)
    {   case Op::JUMP:
            jump(*parsedOp.singleValue);
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
            draw(*parsedOp.singleValue);
            break;
        default:
            // nothing to do here
            break;
    }
}
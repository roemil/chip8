#pragma once
#include <array>

static constexpr uint16_t RAM_SIZE = 4096;
class Chip8
{
    public:
        Chip8();
        ~Chip8() = default;

        Chip8(const Chip8&) = delete;
        Chip8(Chip8&&) = delete;
        Chip8& operator=(const Chip8&) = delete;
    
    private:
        inline constexpr void setFontSprite();

        std::array<uint16_t, RAM_SIZE> memory_{};
        int pc{};
        std::array<uint16_t, RAM_SIZE> stack_{};
        int delayTimer_{};
        int soundTimer_{};

};
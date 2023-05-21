#pragma once

#include <array>

class IOpParser;
class IDrawer;

static constexpr uint16_t RAM_SIZE = 4096;
class Chip8
{
    using size_type = uint16_t;
    public:
        [[nodiscard]] Chip8(const IOpParser& opParser, const IDrawer& drawer);
        ~Chip8();

        Chip8(const Chip8&) = delete;
        Chip8(Chip8&&) = delete;
        Chip8& operator=(const Chip8&) = delete;

        void parseOp(const size_type op);
        [[nodiscard]] size_type getPc() { return pc;};
        [[nodiscard]] std::array<size_type, 16> getRegisters() const {return registers_;};
        [[nodiscard]] size_type getIndexRegister() const {return indexRegister_;};
    
    private:
        inline constexpr void setFontSprite();
        void clearScreen() const;
        void jump(const size_type newAddress);
        void setRegister(const size_type reg, const size_type value);
        void addToRegister(const uint16_t reg, const uint16_t value);

        std::array<size_type, RAM_SIZE> memory_{};
        std::array<size_type, RAM_SIZE> stack_{};
        size_type pc{};
        std::array<size_type, 16> registers_{};
        size_type indexRegister_{};
        int delayTimer_{};
        int soundTimer_{};
        bool vf{};

        const IOpParser& opParser_;
        const IDrawer& drawer_;

};
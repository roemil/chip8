#pragma once

#include <array>
#include <memory>

class OpParser;
class IDrawer;

static constexpr uint16_t RAM_SIZE = 4096;
static constexpr uint16_t program_memory_start = 512;
class Chip8
{
    public:
        using size_type = uint16_t;
        [[nodiscard]] Chip8(const OpParser& opParser, const IDrawer& drawer);
        ~Chip8() = default;

        Chip8(const Chip8&) = delete;
        Chip8(Chip8&&) = delete;
        Chip8& operator=(const Chip8&) = delete;

        void parseOp(const size_type op);
        [[nodiscard]] size_type getPc() { return pc;};
        [[nodiscard]] std::array<size_type, 16> getRegisters() const {return registers_;};
        [[nodiscard]] size_type getIndexRegister() const {return indexRegister_;};
        void run();

        template<std::size_t Size>
        constexpr void load_program(const std::array<uint16_t, Size>& program)
        {
            static_assert(Size <= RAM_SIZE - program_memory_start);
            for(int i = 0; i < program.size(); ++i)
            {
                memory_[program_memory_start + i] = program[i];
            }
        }
    
    private:
        inline constexpr void setFontSprite();
        void clearScreen();
        void jump(const size_type newAddress);
        void setRegister(const size_type reg, const size_type value);
        void addToRegister(const uint16_t reg, const uint16_t value);
        void draw(const uint16_t drawInstructions);
        constexpr uint16_t nextInstruction() const;

        std::array<size_type, RAM_SIZE> memory_{};
        std::array<size_type, RAM_SIZE> stack_{};
        size_type pc{};
        std::array<size_type, 16> registers_{};
        size_type indexRegister_{};
        int delayTimer_{};
        int soundTimer_{};
        bool vf_{};
        std::array<std::array<uint16_t, 64>, 32> pixels{};

        const OpParser& opParser_;
        const IDrawer& drawer_;
};
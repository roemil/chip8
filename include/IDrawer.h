#pragma once
#include <cstdint>
#include <array>

class IDrawer
{
    public:
        virtual ~IDrawer() = default;
        virtual void draw(const std::array<std::array<uint16_t, 64>, 32>& pixelMatrix) const = 0;
};
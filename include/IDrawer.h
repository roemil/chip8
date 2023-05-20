#pragma once
#include <cstdint>

class IDrawer
{
    public:
        virtual ~IDrawer() = default;
        virtual void draw() const = 0;
};
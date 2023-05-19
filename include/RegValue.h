#pragma once

#include <cstdint>
#include <optional>

struct RegValue
{
    constexpr RegValue() = default;
    constexpr RegValue(const uint16_t reg, const uint16_t value) : reg{reg}, value{value} {};
    uint16_t reg{};
    uint16_t value{};

    bool operator==(const RegValue& rh) const
    {
        return reg == rh.reg && value == rh.value;
    }

    bool operator!=(const RegValue& rh) 
    {
        return !(*this == rh);
    }
};
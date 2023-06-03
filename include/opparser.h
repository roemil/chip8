#pragma once

#include <cstdint>

enum class Op;
struct ParsedOpResults;

class OpParser
{
public:
    constexpr ~OpParser() = default;
    constexpr OpParser() = default;

    [[nodiscard]] static constexpr ParsedOpResults parseOp(const uint16_t op);
};
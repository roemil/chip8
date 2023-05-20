#pragma once
#include <cstdint>

enum class Op;
struct ParsedOpResults;

class IOpParser
{
    public:
        virtual ~IOpParser() = default;
        virtual constexpr ParsedOpResults parseOp(const uint16_t op) const = 0;
};
#pragma once
#include <cstdint>
#include <tuple>

enum class Op;

class IOpParser
{
    public:
        virtual ~IOpParser() = default;
        virtual std::tuple<Op, uint16_t, uint16_t> parseOp(const uint16_t op) const = 0;
};
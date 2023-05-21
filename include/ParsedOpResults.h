#pragma once

#include "defs.h"
#include "RegValue.h"

#include <optional>
#include <cstdint>

struct ParsedOpResults
{
    Op op = Op::INVALID;
    std::optional<uint16_t> singleValue;
    std::optional<RegValue> regValue;

    constexpr explicit ParsedOpResults(const Op& op) : op{op}, singleValue{std::nullopt}, regValue{std::nullopt} {};
    constexpr ParsedOpResults(const Op& op, const uint16_t singleValue) : op{op}, singleValue{singleValue}, regValue{std::nullopt} {};
    constexpr ParsedOpResults(const Op& op, const RegValue& regValue) : op{op}, singleValue{std::nullopt}, regValue{regValue} {};

    constexpr inline bool operator==(const ParsedOpResults& rh) const
    {
        if(op != rh.op)
        {
            return false;
        }
        if(singleValue)
        {
            return singleValue == rh.singleValue;
        }
        if(regValue)
        {
            return *regValue == *rh.regValue;
        }
        return true;
    }
};
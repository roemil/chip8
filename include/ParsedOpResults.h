#pragma once

#include "defs.h"
#include "RegValue.h"

#include <optional>
#include <cstdint>

struct ParsedOpResults
{
    Op op = Op::INVALID;
    std::optional<uint16_t> address;
    std::optional<RegValue> regValue;

    explicit ParsedOpResults(const Op& op) : op{op}, address{std::nullopt}, regValue{std::nullopt} {};
    ParsedOpResults(const Op& op, const uint16_t address) : op{op}, address{address}, regValue{std::nullopt} {};
    ParsedOpResults(const Op& op, const RegValue& regValue) : op{op}, address{std::nullopt}, regValue{regValue} {};

    constexpr inline bool operator==(const ParsedOpResults& rh) const
    {
        if(op != rh.op)
        {
            return false;
        }
        if(address)
        {
            return address == rh.address;
        }
        if(regValue)
        {
            return *regValue == *rh.regValue;
        }
        return true;
    }
};
#pragma once

#include "defs.h"
#include "RegValue.h"

#include <optional>
#include <cstdint>
#include <variant>

struct ParsedOpResults
{
    Op op = Op::INVALID;
    std::variant<std::monostate, uint16_t, RegValue> parseResult_;

    constexpr explicit ParsedOpResults(const Op& op) : op{op} {};
    constexpr ParsedOpResults(const Op& op, const uint16_t singleValue) : op{op}, parseResult_{singleValue} {};
    constexpr ParsedOpResults(const Op& op, const RegValue& regValue) : op{op}, parseResult_{regValue} {};

    constexpr inline bool operator==(const ParsedOpResults& rh) const
    {
        return op == rh.op && parseResult_ == rh.parseResult_;
    }
};
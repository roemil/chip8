#pragma once
#include "iopparser.h"
#include <tuple>
#include <optional>

enum class Op;

class OpParser : public IOpParser
{
public:
    ~OpParser() override = default;
    OpParser() = default;

    [[nodiscard]] std::tuple<Op, uint16_t, uint16_t> parseOp(const uint16_t op) const override;
};
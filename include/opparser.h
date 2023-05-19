#pragma once
#include "iopparser.h"

enum class Op;
struct ParsedOpResults;

class OpParser : public IOpParser
{
public:
    ~OpParser() override = default;
    OpParser() = default;

    [[nodiscard]]  ParsedOpResults parseOp(const uint16_t op) const override;
};
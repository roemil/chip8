#pragma once

#include "RegValue.h"

#include <optional>
#include <tuple>
enum class Op
{
    CLEAR_SCREEN,
    JUMP,
    SET_REGISTER,
    ADD_REGISTER,
    SET_INDEX_REGISTER,
    DRAW,
    INVALID
};
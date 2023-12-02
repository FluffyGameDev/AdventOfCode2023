#pragma once

#include "types.h"

namespace AoC::Math
{
    template <typename T>
    s32 Sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }
}
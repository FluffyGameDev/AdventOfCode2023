#pragma once

#include "types.h"

namespace AoC
{
    struct Vector2
    {
        s32 x;
        s32 y;

        static s32 DotProduct(const Vector2& v1, const Vector2& v2)
        {
            return v1.x * v2.x + v1.y * v2.y;
        }

        static s32 SquareDistance(const Vector2& v1, const Vector2& v2)
        {
            Vector2 diff{ v1.x - v2.x, v1.y - v2.y };
            return DotProduct(diff, diff);
        }
    };
}
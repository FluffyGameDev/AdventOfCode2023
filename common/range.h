#pragma once

#include "types.h"

namespace AoC
{
    class Range
    {
    public:
        Range()
            : Range(0, 0)
        {
        }

        Range(s32 min, s32 max)
        {
            Min = min;
            Max = max;
        }

        bool IsInRange(s32 value) const
        {
            return (Min <= value && value <= Max);
        }

        bool IsInRange(const Range& range) const
        {
            return IsInRange(range.Min) && IsInRange(range.Max);
        }

        bool DoesRangeOverlap(const Range& range) const
        {
            return IsInRange(range.Min) || IsInRange(range.Max) || range.IsInRange(Min) || range.IsInRange(Max);
        }

        s32 Min;
        s32 Max;
    };

    bool operator==(const Range& lhs, const Range& rhs)
    {
        return lhs.Min == rhs.Min && lhs.Max == rhs.Max;
    }
}
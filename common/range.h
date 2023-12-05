#pragma once

#include "types.h"

namespace AoC
{
    template<typename T>
    class Range
    {
    public:
        Range()
            : Range(0, 0)
        {
        }

        Range(T min, T max)
        {
            Min = min;
            Max = max;
        }

        bool IsInRange(T value) const
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

        T Min;
        T Max;
    };

    using RangeU64 = Range<u64>;
}
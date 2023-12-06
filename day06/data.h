#pragma once

#include <vector>
#include "types.h"

namespace AoC
{
    const char* s_testInputData
    {
        "Time:      7  15   30\n"
        "Distance : 9  40  200"
    };

    struct InputData
    {
        std::vector<u64> Times;
        std::vector<u64> Distances;
    };

    struct OutputData
    {
        u64 ProductNumberWays{};
        u64 SingleRaceNumberWays{};
    };
}
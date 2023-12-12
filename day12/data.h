#pragma once

#include <string>
#include <vector>
#include "types.h"

namespace AoC
{
    const char* s_testInputData
    {
        "???.### 1,1,3\n"
        ".??..??...?##. 1,1,3\n"
        "?#?#?#?#?#?#?#? 1,3,1,6\n"
        "????.#...#... 4,1,1\n"
        "????.######..#####. 1,6,5\n"
        "?###???????? 3,2,1"
    };

    struct LineData
    {
        std::string RawData;
        std::vector<u32> BlockData;
    };

    struct InputData
    {
        std::vector<LineData> Lines{};
    };

    struct OutputData
    {
        u32 PossibleArrangementsCount{};
    };
}
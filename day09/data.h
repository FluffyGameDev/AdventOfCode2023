#pragma once

#include "types.h"
#include <vector>

namespace AoC
{
    const char* s_testInputData
    {
        "0 3 6 9 12 15\n"
        "1 3 6 10 15 21\n"
        "10 13 16 21 30 45"
    };

    struct InputData
    {
        std::vector<std::vector<s32>> HistoryLines;
    };

    struct OutputData
    {
        s32 SumPredictions{};
        s32 SumPastValues{};
    };
}
#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "types.h"

namespace AoC
{
    const char* s_testInputData
    {
        "rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7"
    };

    struct InputData
    {
        std::string RawCommands;
        std::vector<std::string_view> Commands;
    };

    struct OutputData
    {
        u64 SumHashes{};
        u64 TotalFocusingPower{};
    };
}
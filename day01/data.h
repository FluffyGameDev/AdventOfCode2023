#pragma once

#include <string>
#include <vector>

#include "types.h"

namespace AoC
{
    const char* s_testInputData
    {
        "two1nine\n"
        "eightwothree\n"
        "abcone2threexyz\n"
        "xtwone3four\n"
        "4nineeightseven2\n"
        "zoneight234\n"
        "7pqrstsixteen\n"
    };

    struct InputData
    {
        std::vector<std::string> Lines;
    };

    struct OutputData
    {
        u64 SumFirstLastDigits{};
    };
}
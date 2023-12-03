#pragma once

#include <vector>
#include "types.h"

namespace AoC
{
    const char* s_testInputData
    {
        "467..114..\n"
        "...*......\n"
        "..35..633.\n"
        "......#...\n"
        "617*......\n"
        ".....+.58.\n"
        "..592.....\n"
        "......755.\n"
        "...$.*....\n"
        ".664.598.."
    };

    struct InputData
    {
        u32 GridWidth;
        u32 GridHeight;
        std::vector<char> Grid;
    };

    struct OutputData
    {
        u64 SumAllPartNumbers;
        u64 SumGearRatios;
    };
}
#pragma once

#include <string>
#include "vector2.h"

namespace AoC
{
    const char* s_testInputData
    {
        ".|...\\....\n"
        "|.-.\\.....\n"
        ".....|-...\n"
        "........|.\n"
        "..........\n"
        ".........\\\n"
        "..../.\\\\..\n"
        ".-.-/..|..\n"
        ".|....-|.\\\n"
        "..//.|...."
    };

    struct Grid
    {
        std::string Data;
        Vector2 Size;
    };

    struct InputData
    {
        Grid Map;
    };

    struct OutputData
    {
        u32 EngergizedCells{};
        u32 BestEngergizedCells{};
    };
}
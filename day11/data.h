#pragma once

#include <vector>
#include "types.h"
#include "vector2.h"

namespace AoC
{
    const char* s_testInputData
    {
        "...#......\n"
        ".......#..\n"
        "#.........\n"
        "..........\n"
        "......#...\n"
        ".#........\n"
        ".........#\n"
        "..........\n"
        ".......#..\n"
        "#...#....."
    };

    struct InputData
    {
        std::vector<Vector2> Galaxies;
        Vector2 UniverseSize;
    };

    struct OutputData
    {
        u64 SumShortestPath{};
    };
}
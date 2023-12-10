#pragma once

#include <vector>
#include "types.h"
#include "vector2.h"

namespace AoC
{
    const char* s_testInputDataPart1
    {
        "..F7.\n"
        ".FJ|.\n"
        "SJ.L7\n"
        "|F--J\n"
        "LJ..."
    };

    const char* s_testInputDataPart2
    {
        "FF7FSF7F7F7F7F7F---7\n"
        "L|LJ||||||||||||F--J\n"
        "FL-7LJLJ||||||LJL-77\n"
        "F--JF--7||LJLJ7F7FJ-\n"
        "L---JF-JLJ.||-FJLJJ7\n"
        "|F|F-JF---7F7-L7L|7|\n"
        "|FFJF7L7F-JF7|JL---7\n"
        "7-L-JL7||F7|L7F-7F7|\n"
        "L.L7LFJ|||||FJL7||LJ\n"
        "L7JLJL-JLJLJL--JLJ.L"
    };

    enum class DirectionFlags : u8
    {
        None = 0x00,
        North = 0x01,
        West = 0x02,
        South = 0x04,
        East = 0x08,

        Horizontal = West | East,
        Vertical = North | South,

        CornerSE = South | East,
        CornerNE = North | East,
        CornerSW = South | West,
        CornerNW = North | West,

        All = North | South | West | East
    };

    struct InputData
    {
        std::vector<DirectionFlags> Grid;
        Vector2 GridSize;
    };

    struct OutputData
    {
        u32 FurthestPointLoopDistance{};
        u32 LeftTileCount{};
        u32 RightTileCount{};
    };
}
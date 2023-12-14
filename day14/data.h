#pragma once

#include <string>
#include "vector2.h"

namespace AoC
{
    const char* s_testInputData
    {
        "O....#....\n"
        "O.OO#....#\n"
        ".....##...\n"
        "OO.#O....O\n"
        ".O.....O#.\n"
        "O.#..O.#.#\n"
        "..O..#O..O\n"
        ".......O..\n"
        "#....###..\n"
        "#OO..#...."
    };

    struct Grid
    {
        std::string Buffer;
        Vector2 Size;
    };

    struct InputData
    {
        Grid Map;
    };

    struct OutputData
    {
        u64 NorthBeamLoad{};
    };
}
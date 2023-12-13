#pragma once

#include <vector>
#include "types.h"
#include "vector2.h"

namespace AoC
{
    const char* s_testInputData
    {
        "#.##..##.\n"
        "..#.##.#.\n"
        "##......#\n"
        "##......#\n"
        "..#.##.#.\n"
        "..##..##.\n"
        "#.#.##.#.\n"
        "\n"
        "#...##..#\n"
        "#....#..#\n"
        "..##..###\n"
        "#####.##.\n"
        "#####.##.\n"
        "..##..###\n"
        "#....#..#"
    };

    struct Grid
    {
        std::vector<char> Buffer;
        Vector2 Size;
    };

    struct InputData
    {
        std::vector<Grid> Grids;
    };

    struct OutputData
    {
        u32 MidRowColumnCount{};
    };
}
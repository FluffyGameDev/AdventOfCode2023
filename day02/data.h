#pragma once

#include <vector>
#include "types.h"

namespace AoC
{
    const char* s_testInputData
    {
        "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n"
        "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\n"
        "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\n"
        "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\n"
        "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"
    };

    struct GameSubset
    {
        u32 Red;
        u32 Green;
        u32 Blue;
    };

    struct GameResult
    {
        std::vector<GameSubset> Subsets;
        u32 GameId;
    };

    struct InputData
    {
        std::vector<GameResult> GameResults;
        GameSubset Max;
    };

    struct OutputData
    {
        u32 SumValidGames;
        u32 SumMaxProduct;
    };
}
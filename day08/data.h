#pragma once

#include <string>
#include <unordered_map>
#include "types.h"

namespace AoC
{
    const char* s_testInputDataPart1
    {
        "LLR\n"
        "\n"
        "AAA = (BBB, BBB)\n"
        "BBB = (AAA, ZZZ)\n"
        "ZZZ = (ZZZ, ZZZ)"
    };

    const char* s_testInputDataPart2
    {
        "LR\n"
        "\n"
        "11A = (11B, XXX)\n"
        "11B = (XXX, 11Z)\n"
        "11Z = (11B, XXX)\n"
        "22A = (22B, XXX)\n"
        "22B = (22C, 22C)\n"
        "22C = (22Z, 22Z)\n"
        "22Z = (22B, 22B)\n"
        "XXX = (XXX, XXX)"
    };

    struct Node
    {
        u32 NodeId;
        u32 LeftId;
        u32 RightId;
    };

    struct InputData
    {
        std::string DirectionsBuffer{};
        std::unordered_map<u32, Node> Nodes;
    };

    struct OutputData
    {
        u64 StepsToEnd{};
    };
}
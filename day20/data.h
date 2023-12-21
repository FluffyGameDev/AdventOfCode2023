#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "types.h"

namespace AoC
{
    const char* s_testInputData1
    {
        "broadcaster -> a, b, c\n"
        "%a -> b\n"
        "%b -> c\n"
        "%c -> inv\n"
        "&inv -> a"
    };

    const char* s_testInputData2
    {
        "broadcaster -> a\n"
        "%a -> inv, con\n"
        "&inv -> b\n"
        "%b -> con\n"
        "&con -> output"
    };

    enum class ModuleType
    {
        Broadcaster,
        FlipFlop,
        Conjuction
    };

    struct ModuleDescriptor
    {
        std::vector<u64> Destinations;
        std::string Name;
        ModuleType ModuleType;
    };

    struct InputData
    {
        std::vector<ModuleDescriptor> Modules;
        std::unordered_map<std::string, u64> ModuleIds;
    };

    struct OutputData
    {
        u64 LowPulseCount{};
        u64 HighPulseCount{};
    };
}
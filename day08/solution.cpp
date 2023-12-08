#include "data.h"
#include "aoc.h"

#include <numeric>
#include <string_view>

namespace AoC
{
    namespace Internal
    {
        u32 ConvertToNodeId(const std::string_view& node)
        {
            u32 nodeId{};
            for (char c : node)
            {
                nodeId = (nodeId << 8) | (u32)c;
            }
            return nodeId;
        }

        u64 ComputeStepsUntilEnd(u32 currentNodeId, u32 endNodeId, u32 nodeMask, const InputData& input)
        {
            u64 steps{};
            while ((currentNodeId & nodeMask) != (endNodeId & nodeMask))
            {
                const Node& currentNode{ input.Nodes.at(currentNodeId) };
                char currentCommand{ input.DirectionsBuffer[steps % input.DirectionsBuffer.size()] };
                switch (currentCommand)
                {
                    case 'L': currentNodeId = currentNode.LeftId; break;
                    case 'R': currentNodeId = currentNode.RightId; break;
                    default: fmt::print("Invalid direction."); return 0;
                }
                ++steps;
            }
            return steps;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        std::getline(inputStream, inputData.DirectionsBuffer);

        std::getline(inputStream, line);
        while (std::getline(inputStream, line))
        {
            std::string_view node(line.c_str(), 3);
            std::string_view left(line.c_str() + 7, 3);
            std::string_view right(line.c_str() + 12, 3);

            u32 nodeId{ Internal::ConvertToNodeId(node) };
            Node newNode
            {
                nodeId,
                Internal::ConvertToNodeId(left),
                Internal::ConvertToNodeId(right)
            };
            inputData.Nodes[nodeId] = newNode;
        }

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        std::vector<u32> startNodeIds;

        u32 nodeMask{};
        u32 startNodeId{ Internal::ConvertToNodeId("AAA") };
        u32 endNodeId{ Internal::ConvertToNodeId("ZZZ") };
        if (context.PartNumber == 1)
        {
            startNodeIds.push_back(startNodeId);
            nodeMask = 0xffffff;
        }
        else
        {
            nodeMask = 0xff;
            for (const auto& [nodeId, node] : input.Nodes)
            {
                if ((nodeId & nodeMask) == (startNodeId & nodeMask))
                {
                    startNodeIds.push_back(nodeId);
                }
            }
        }

        output.StepsToEnd = 0;
        for (u32& startNode : startNodeIds)
        {
            u64 steps{ Internal::ComputeStepsUntilEnd(startNode, endNodeId, nodeMask, input) };
            if (output.StepsToEnd == 0)
            {
                output.StepsToEnd = steps;
            }
            else
            {
                output.StepsToEnd = std::lcm(output.StepsToEnd, steps);
            }
        }
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.StepsToEnd == 6;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Steps to End: {}\n", output.StepsToEnd);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputDataPart1, AoC::s_testInputDataPart2 });
}
#include "data.h"
#include "aoc.h"

namespace AoC
{
    namespace Internal
    {
        u64 ComputeNumberPossibleVictories(u64 time, u64 distance)
        {
            double minPressTime{ ((time - std::sqrt(time * time - 4 * distance)) / 2) };
            double maxPressTime{ ((time + std::sqrt(time * time - 4 * distance)) / 2) };

            minPressTime = std::ceil(minPressTime + 0.0001);
            maxPressTime = std::floor(maxPressTime - 0.0001);

            return (u64)(maxPressTime - minPressTime) + 1ULL;
        }

        u64 ConcatenateNumbers(const std::vector<u64>& numbers)
        {
            u64 value{ 0 };
            for (u64 number : numbers)
            {
                u64 multiplier{ 1 };
                while (multiplier < number)
                {
                    multiplier *= 10;
                }

                value = value * multiplier + number;
            }
            return value;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        std::string line{};
        if (std::getline(inputStream, line))
        {
            std::stringstream lineStream{ line };
            lineStream.seekg(10);

            u64 number{};
            while (lineStream >> number)
            {
                inputData.Times.push_back(number);
            }
        }
        else
        {
            return false;
        }

        if (std::getline(inputStream, line))
        {
            std::stringstream lineStream{ line };
            lineStream.seekg(10);

            u64 number{};
            while (lineStream >> number)
            {
                inputData.Distances.push_back(number);
            }
        }
        else
        {
            return false;
        }

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        output.ProductNumberWays = 1;
        u64 raceCount{ (u64)input.Distances.size() };
        for (u32 i = 0; i < raceCount; ++i)
        {
            output.ProductNumberWays *= Internal::ComputeNumberPossibleVictories(input.Times[i], input.Distances[i]);
        }

        u64 realTime{ Internal::ConcatenateNumbers(input.Times) };
        u64 realDistance{ Internal::ConcatenateNumbers(input.Distances) };
        output.SingleRaceNumberWays = Internal::ComputeNumberPossibleVictories(realTime, realDistance);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.ProductNumberWays == 288;
        didTestsPass &= output.SingleRaceNumberWays == 71503;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Product Number Ways to beat record: {}\n", output.ProductNumberWays);
        fmt::print("Single Race Number Ways to beat record: {}\n", output.SingleRaceNumberWays);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
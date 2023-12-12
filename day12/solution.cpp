#include "data.h"
#include "aoc.h"

#include <numeric>

namespace AoC
{
    namespace Internal
    {
        // Solution shamelessly stolen from https://pastebin.com/nUfatbwu
        u64 Solve(size_t currentChar, size_t currentBlock, bool placed, std::vector<std::vector<std::vector<u64>>>& dp,
            const std::string& rawLine, const std::vector<u32>& blocks)
        {
            size_t rawLineSize{ rawLine.size() };
            size_t blocksCount{ blocks.size() };

            if ((currentChar > rawLineSize + 1) || (currentChar > rawLineSize && !placed))
                return dp[currentChar][currentBlock][placed] = 0;

            if (dp[currentChar][currentBlock][placed] != -1)
                return dp[currentChar][currentBlock][placed];

            if (placed)
            {
                for (size_t k = currentChar - blocks[currentBlock - 1] - 1; k < currentChar - 1; k++)
                {
                    if (rawLine[k] == '.')
                    {
                        return dp[currentChar][currentBlock][placed] = 0;
                    }
                }
                if (currentChar != rawLineSize + 1)
                {
                    if (rawLine[currentChar - 1] == '#')
                    {
                        return dp[currentChar][currentBlock][placed] = 0;
                    }
                }
            }
            if (currentBlock == blocksCount)
            {
                bool foundNonDefective = true;
                for (size_t k = currentChar; k < rawLineSize; k++)
                {
                    foundNonDefective &= (rawLine[k] != '#');
                }
                return dp[currentChar][currentBlock][placed] = foundNonDefective;
            }

            u64 arragnementCount = 0;
            if (currentChar < rawLineSize)
            {
                if (rawLine[currentChar] != '.')
                {
                    arragnementCount += Solve(currentChar + blocks[currentBlock] + 1, currentBlock + 1, true, dp, rawLine, blocks);
                }
                if (rawLine[currentChar] != '#')
                {
                    arragnementCount += Solve(currentChar + 1, currentBlock, false, dp, rawLine, blocks);
                }
            }
            return dp[currentChar][currentBlock][placed] = arragnementCount;
        }

        u64 ComputeArrangementsCount(const LineData& line, u32 folds)
        {
            std::string rawLine{ line.RawData };
            std::vector<u32> blocks{ line.BlockData };
            for (u32 i = 1; i < folds; ++i)
            {
                rawLine += '?';
                rawLine += line.RawData;
                blocks.insert(blocks.end(), line.BlockData.begin(), line.BlockData.end());
            }

            std::vector<std::vector<std::vector<u64>>> dp(2 * rawLine.size() + 1, std::vector<std::vector<u64>>(blocks.size() + 1, std::vector<u64>(2, -1)));
            return Solve(0, 0, false, dp, rawLine, blocks);
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        char ignore{};
        while (std::getline(inputStream, line))
        {
            std::stringstream linestream{ line };
            LineData& newLine{ inputData.Lines.emplace_back() };

            linestream >> newLine.RawData;

            u32 number{};
            while (linestream >> number)
            {
                newLine.BlockData.push_back(number);
                linestream >> ignore;
            }
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        u32 folds{ context.PartNumber == 1 ? 1U : 5U };
        auto countPossibilities{ [folds](u64 total, const LineData& line) { return total + Internal::ComputeArrangementsCount(line, folds); } };
        output.PossibleArrangementsCount = std::accumulate(input.Lines.begin(), input.Lines.end(), 0ULL, countPossibilities);
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        if (context.PartNumber == 1)
        {
            didTestsPass &= output.PossibleArrangementsCount == 21;
        }
        else
        {
            didTestsPass &= output.PossibleArrangementsCount == 525152;
        }

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Possible Arrangements Count: {}\n", output.PossibleArrangementsCount);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData, AoC::s_testInputData });
}
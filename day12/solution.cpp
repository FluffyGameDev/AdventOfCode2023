#include "data.h"
#include "aoc.h"

#include <numeric>

namespace AoC
{
    namespace Internal
    {
        u32 CountNextBlockSize(std::string::const_iterator& rawLineIt, std::string::const_iterator endIt)
        {
            while (rawLineIt != endIt && *rawLineIt == '.')
            {
                ++rawLineIt;
            }

            u32 blockSize{};
            while (rawLineIt != endIt && *rawLineIt == '#')
            {
                ++rawLineIt;
                ++blockSize;
            }
            return blockSize;
        }

        bool IsArrangementPossible(const std::vector<u32>& blocks, const std::string& rawLine)
        {
            auto blocksIt{ blocks.begin() };
            auto blocksEndIt{ blocks.end() };
            auto rawLineIt{ rawLine.begin() };
            auto rawLineEndIt{ rawLine.end() };
            while (rawLineIt != rawLineEndIt && blocksIt != blocksEndIt)
            {
                u32 blockSize{ CountNextBlockSize(rawLineIt, rawLineEndIt) };
                if (blockSize != *blocksIt)
                {
                    return blockSize < *blocksIt && rawLineIt != rawLineEndIt && *rawLineIt == '?';
                }
                ++blocksIt;
            }
            return CountNextBlockSize(rawLineIt, rawLineEndIt) == 0 && blocksIt == blocksEndIt;
        }

        void TryPossibleArrangements(const std::vector<u32>& blocks, size_t currentIndex, std::string& rawLine, u32& arrangementsCount)
        {
            size_t nextUnknownIndex{ rawLine.find_first_of('?', currentIndex) };
            if (nextUnknownIndex != std::string::npos)
            {
                rawLine[nextUnknownIndex] = '.';
                if (IsArrangementPossible(blocks, rawLine))
                {
                    TryPossibleArrangements(blocks, nextUnknownIndex + 1, rawLine, arrangementsCount);
                }

                rawLine[nextUnknownIndex] = '#';
                if (IsArrangementPossible(blocks, rawLine))
                {
                    TryPossibleArrangements(blocks, nextUnknownIndex + 1, rawLine, arrangementsCount);
                }

                rawLine[nextUnknownIndex] = '?';
            }
            else if(IsArrangementPossible(blocks, rawLine))
            {
                ++arrangementsCount;
            }
        }

        u32 ComputeArrangementsCount(const LineData& line, u32 folds)
        {
            std::string rawLine{ line.RawData };
            std::vector<u32> blocks{ line.BlockData };
            for (u32 i = 1; i < folds; ++i)
            {
                rawLine += '?';
                rawLine += line.RawData;
                blocks.insert(blocks.end(), line.BlockData.begin(), line.BlockData.end());
            }
            fmt::print("Testing Line: {}", rawLine);

            u32 arrangementsCount{};
            TryPossibleArrangements(blocks, 0, rawLine, arrangementsCount);
            fmt::print(" -> {} arrangements\n", arrangementsCount);
            return arrangementsCount;
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
        auto countPossibilities{ [folds](u32 total, const LineData& line) { return total + Internal::ComputeArrangementsCount(line, folds); } };
        output.PossibleArrangementsCount = std::accumulate(input.Lines.begin(), input.Lines.end(), 0U, countPossibilities);
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
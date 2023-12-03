#include "data.h"
#include "aoc.h"

#include <numeric>
#include <unordered_map>

namespace AoC
{
    namespace Internal
    {
        struct PartNumber
        {
            u32 Value;
            u32 ValueStartIndex;
            u32 SymbolIndex;
            char Symbol;
        };

        struct GearRatio
        {
            u32 Ratio;
            u32 PartCount;
        };

        bool ExtractSymbol(const InputData& input, u32 index, PartNumber& partNumber)
        {
            char c{ input.Grid[index] };
            bool canExtractSymbol{ c != '.' && !std::isdigit(c) };
            if (canExtractSymbol)
            {
                partNumber.Symbol = c;
                partNumber.SymbolIndex = index;
            }
            return canExtractSymbol;
        }

        void ExtractPartNumbers(const InputData& input, std::vector<PartNumber>& partNumbers)
        {
            u32 gridSize{ (u32)input.Grid.size() };
            for (u32 flatIndex = 0; flatIndex < gridSize; ++flatIndex)
            {
                if (std::isdigit(input.Grid[flatIndex]))
                {
                    PartNumber& newPartNumber{ partNumbers.emplace_back() };
                    newPartNumber.ValueStartIndex = flatIndex;

                    u32 numberWidth{};
                    u32 x{ flatIndex % input.GridWidth };
                    u32 y{ flatIndex / input.GridWidth };
                    u32 startIndex{ flatIndex };
                    while (std::isdigit(input.Grid[flatIndex]) && flatIndex / input.GridWidth == y)
                    {
                        newPartNumber.Value = newPartNumber.Value * 10 + (input.Grid[flatIndex] - '0');
                        ++flatIndex;
                        ++numberWidth;
                    }
                    --flatIndex;

                    u32 minX{ (u32)std::max((s32)x - 1, 0) };
                    u32 maxX{ std::min(x + numberWidth + 1, input.GridWidth) };
                    if (y > 0)
                    {
                        u32 minIndex{ (y - 1) * input.GridWidth + minX };
                        u32 maxIndex{ (y - 1) * input.GridWidth + maxX };
                        for (u32 i = minIndex; i < maxIndex; ++i)
                        {
                            if (ExtractSymbol(input, i, newPartNumber))
                            {
                                break;
                            }
                        }
                    }

                    if (x > 0) ExtractSymbol(input, x - 1 + y * input.GridWidth, newPartNumber);
                    if (x < input.GridWidth - 1) ExtractSymbol(input, x + numberWidth + y * input.GridWidth, newPartNumber);

                    if (y < input.GridHeight - 1)
                    {
                        u32 minIndex{ (y + 1) * input.GridWidth + minX };
                        u32 maxIndex{ (y + 1) * input.GridWidth + maxX };
                        for (u32 i = minIndex; i < maxIndex; ++i)
                        {
                            if (ExtractSymbol(input, i, newPartNumber))
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }

        void ComputeGearRatios(const std::vector<PartNumber>& partNumbers, std::unordered_map<u32, GearRatio>& gearRatios)
        {
            for (const Internal::PartNumber& partNumber : partNumbers)
            {
                if (partNumber.Symbol == '*')
                {
                    auto foundIt{ gearRatios.find(partNumber.SymbolIndex) };
                    if (foundIt != gearRatios.end())
                    {
                        foundIt->second.Ratio *= partNumber.Value;
                        ++foundIt->second.PartCount;
                    }
                    else
                    {
                        gearRatios[partNumber.SymbolIndex] = { partNumber.Value, 1 };
                    }
                }
            }
        }

        u32 AccumulatePartNumbers(u32 total, const PartNumber& partNumber)
        {
            if (partNumber.Symbol != 0)
            {
                total += partNumber.Value;
            }
            return total;
        }

        u32 AccumulateGearRatios(u32 total, const std::pair<u32, GearRatio>& gearRatio)
        {
            if (gearRatio.second.PartCount == 2)
            {
                total += gearRatio.second.Ratio;
            }
            return total;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        std::string line{};
        while (std::getline(inputStream, line))
        {
            inputData.GridWidth = (u32)line.size();
            ++inputData.GridHeight;

            inputData.Grid.insert(inputData.Grid.end(), line.begin(), line.end());
        }

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        std::vector<Internal::PartNumber> partNumbers{};
        ExtractPartNumbers(input, partNumbers);
        output.SumAllPartNumbers = std::accumulate(partNumbers.begin(), partNumbers.end(), 0, Internal::AccumulatePartNumbers);

        std::unordered_map<u32, Internal::GearRatio> gearRatios{};
        Internal::ComputeGearRatios(partNumbers, gearRatios);
        output.SumGearRatios = std::accumulate(gearRatios.begin(), gearRatios.end(), 0, Internal::AccumulateGearRatios);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.SumAllPartNumbers == 4361;
        didTestsPass &= output.SumGearRatios == 467835;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Sum All PArt Numbers: {}\n", output.SumAllPartNumbers);
        fmt::print("Sum Gear Ratios: {}\n", output.SumGearRatios);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
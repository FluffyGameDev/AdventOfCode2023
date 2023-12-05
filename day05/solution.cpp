#include "data.h"
#include "aoc.h"

#include <numeric>

namespace AoC
{
    namespace Internal
    {
        bool IsInRange(u64 element, const ConversionRange& range)
        {
            return (element >= range.SourceRangeStart && element < range.SourceRangeStart + range.RangeLength);
        }

        u64 ConvertElement(u64 element, const ConversionMap& conversionMap)
        {
            for (const auto& range : conversionMap.Ranges)
            {
                if (IsInRange(element, range))
                {
                    return element - range.SourceRangeStart + range.DestinationRangeStart;
                }
            }

            return element;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        std::string ignore{};
        std::string line{};
        if (std::getline(inputStream, line))
        {
            std::stringstream lineStream{ line };
            lineStream >> ignore;

            u64 seed{};
            while (lineStream >> seed)
            {
                inputData.Seeds.push_back(seed);
            }

            std::getline(inputStream, line);
        }
        else
        {
            return false;
        }

        while (std::getline(inputStream, line))
        {
            ConversionMap& newConversionMap{ inputData.ConversionMaps.emplace_back() };
            while (std::getline(inputStream, line) && !line.empty())
            {
                ConversionRange& newRange{ newConversionMap.Ranges.emplace_back() };
                std::stringstream lineStream{ line };
                lineStream >> newRange.DestinationRangeStart >> newRange.SourceRangeStart >> newRange.RangeLength;
            }
        }

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        std::vector<u64> currentStep{ input.Seeds };
        for (const auto& conversionMap : input.ConversionMaps)
        {
            std::transform(currentStep.begin(), currentStep.end(), currentStep.begin(),
                [&conversionMap](u64 element) { return Internal::ConvertElement(element, conversionMap); });
        }

        output.LowestLocationNumber = std::accumulate(currentStep.begin(), currentStep.end(), std::numeric_limits<u64>::max(),
            [](u64 currentMin, u64 element) { return std::min(currentMin, element); });
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.LowestLocationNumber == 35;
        didTestsPass &= output.LowestLocationNumberRanges == 46;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Lowest Location Number: {}\n", output.LowestLocationNumber);
        fmt::print("Lowest Location Number with Ranges: {}\n", output.LowestLocationNumberRanges);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
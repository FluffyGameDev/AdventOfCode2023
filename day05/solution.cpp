#include "data.h"
#include "aoc.h"

#include <numeric>
#include <stack>

namespace AoC
{
    namespace Internal
    {
        u64 TransformElement(u64 element, const ConversionRange& conversionRange)
        {
            return element - conversionRange.SourceRange.Min + conversionRange.DestinationRangeStart;
        }

        RangeU64 TransformRange(const RangeU64& sourceRange, const ConversionRange& conversionRange)
        {
            return
            {
                TransformElement(sourceRange.Min, conversionRange),
                TransformElement(sourceRange.Max, conversionRange)
            };
        }

        u64 ConvertElement(u64 element, const ConversionMap& conversionMap)
        {
            for (const auto& range : conversionMap.Ranges)
            {
                if (range.SourceRange.IsInRange(element))
                {
                    return element - range.SourceRange.Min + range.DestinationRangeStart;
                }
            }

            return element;
        }

        u64 ComputeLowestSeedLocation(const InputData& input)
        {
            std::vector<u64> currentStep{ input.Seeds };
            for (const auto& conversionMap : input.ConversionMaps)
            {
                std::transform(currentStep.begin(), currentStep.end(), currentStep.begin(),
                    [&conversionMap](u64 element) { return ConvertElement(element, conversionMap); });
            }

            return std::accumulate(currentStep.begin(), currentStep.end(), std::numeric_limits<u64>::max(),
                [](u64 currentMin, u64 element) { return std::min(currentMin, element); });
        }

        void ComputeMappedRanges(const std::vector<RangeU64>& sourceRanges, std::vector<RangeU64>& destinationRanges,
            const ConversionMap& conversionMap)
        {
            destinationRanges.clear();
            std::stack<RangeU64> subRanges{};
            for (const RangeU64& range : sourceRanges)
            {
                subRanges.push(range);
            }

            while (!subRanges.empty())
            {
                RangeU64 currentRange{ subRanges.top() };
                subRanges.pop();

                bool foundOverlap{};
                for (const auto& mapRange : conversionMap.Ranges)
                {
                    if (mapRange.SourceRange.IsInRange(currentRange))
                    {
                        destinationRanges.push_back(TransformRange(currentRange, mapRange));
                        foundOverlap = true;
                        break;
                    }
                    else if (currentRange.IsInRange(mapRange.SourceRange))
                    {
                        if (currentRange.Min != mapRange.SourceRange.Min)
                        {
                            RangeU64 lowerRange{ currentRange.Min, mapRange.SourceRange.Min - 1ULL };
                            subRanges.push(lowerRange);
                        }

                        if (mapRange.SourceRange.Max != currentRange.Max)
                        {
                            RangeU64 upperRange{ mapRange.SourceRange.Max + 1ULL, currentRange.Max };
                            subRanges.push(upperRange);
                        }

                        destinationRanges.push_back(TransformRange(mapRange.SourceRange, mapRange));
                        foundOverlap = true;
                        break;
                    }
                    else if (mapRange.SourceRange.IsInRange(currentRange.Min))
                    {
                        RangeU64 lowerRange{ currentRange.Min, mapRange.SourceRange.Max };
                        RangeU64 upperRange{ mapRange.SourceRange.Max + 1ULL, currentRange.Max };

                        subRanges.push(upperRange);
                        destinationRanges.push_back(TransformRange(lowerRange, mapRange));
                        foundOverlap = true;
                        break;
                    }
                    else if (mapRange.SourceRange.IsInRange(currentRange.Max))
                    {
                        RangeU64 lowerRange{ currentRange.Min, mapRange.SourceRange.Min - 1ULL };
                        RangeU64 upperRange{ mapRange.SourceRange.Min, currentRange.Max };

                        subRanges.push(lowerRange);
                        destinationRanges.push_back(TransformRange(upperRange, mapRange));
                        foundOverlap = true;
                        break;
                    }
                }

                if (!foundOverlap)
                {
                    destinationRanges.push_back(currentRange);
                }
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
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

                u64 rangeStart{};
                u64 rangeLength{};
                lineStream >> newRange.DestinationRangeStart >> rangeStart >> rangeLength;
                newRange.SourceRange = RangeU64{ rangeStart, rangeStart + rangeLength - 1ULL };
            }
        }

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        output.LowestLocationNumber = Internal::ComputeLowestSeedLocation(input);

        u32 currentRangesIndex{};
        std::vector<RangeU64> ranges[2];
        std::vector<RangeU64>* currentRanges{ &ranges[currentRangesIndex] };

        currentRanges->reserve(input.Seeds.size() / 2);
        for (size_t i = 0; i < input.Seeds.size(); i += 2)
        {
            u64 start{ input.Seeds[i] };
            u64 end{ start + input.Seeds[i + 1ULL] };
            currentRanges->emplace_back(start, end);
        }

        for (const auto& conversionMap : input.ConversionMaps)
        {
            std::vector<RangeU64>* sourceRanges{ currentRanges };
            currentRangesIndex = (currentRangesIndex + 1) % 2;
            currentRanges = &ranges[currentRangesIndex];
            Internal::ComputeMappedRanges(*sourceRanges, *currentRanges, conversionMap);
        }


        output.LowestLocationNumberRanges = std::accumulate(currentRanges->begin(), currentRanges->end(),
            std::numeric_limits<u64>::max(),
            [](u64 currentMin, const RangeU64& range) { return std::min(currentMin, range.Min); });
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
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
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData });
}
#include "data.h"
#include "aoc.h"

#include <numeric>

namespace AoC
{
    namespace Internal
    {
        u32 ComputeCardMatches(const CardData& card)
        {
            const auto& winningNumbers{ card.WinningNumbers };
            auto isWinningNumber{ [&winningNumbers](u32 number)
                { return std::find(winningNumbers.begin(), winningNumbers.end(), number) != winningNumbers.end(); } };

            u32 matches{ (u32)std::count_if(card.CardNumbers.begin(), card.CardNumbers.end(), isWinningNumber) };
            return matches;
        }

        u32 ComputeCardPoints(u32 matches)
        {
            u32 points{ (u32)((1 << matches) >> 1) };
            return points;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        std::string ignore{};
        u32 currentNumber{};
        while (std::getline(inputStream, line))
        {
            std::stringstream lineStream{ line };
            CardData& newCard{ inputData.Cards.emplace_back() };

            lineStream >> ignore;
            lineStream >> newCard.CardId;
            lineStream >> ignore;

            while (lineStream >> currentNumber)
            {
                newCard.CardNumbers.push_back(currentNumber);
            }

            lineStream.clear();
            lineStream >> ignore;

            while (lineStream >> currentNumber)
            {
                newCard.WinningNumbers.push_back(currentNumber);
            }
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        std::vector<u32> cardMatches(input.Cards.size());
        std::vector<u32> cardPoints(input.Cards.size());
        std::transform(input.Cards.begin(), input.Cards.end(), cardMatches.begin(), Internal::ComputeCardMatches);
        std::transform(cardMatches.begin(), cardMatches.end(), cardPoints.begin(), Internal::ComputeCardPoints);
        output.SumCardPoints = std::accumulate(cardPoints.begin(), cardPoints.end(), 0, std::plus{});

        std::vector<u64> cardCount(cardPoints.size(), 1);
        u32 maxIndex{ (u32)cardCount.size() };
        for (u32 i = 0; i < maxIndex; ++i)
        {
            u32 matches{ cardMatches[i] };
            u64 currentCardCount{ cardCount[i] };
            for (u32 j = 1; j <= matches; ++j)
            {
                cardCount[(u64)i + j] += currentCardCount;
            }
        }

        output.TotalCardCount = std::accumulate(cardCount.begin(), cardCount.end(), 0ULL, std::plus{});
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.SumCardPoints == 13;
        didTestsPass &= output.TotalCardCount == 30;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Sum Card Points: {}\n", output.SumCardPoints);
        fmt::print("Total Card Count: {}\n", output.TotalCardCount);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData });
}
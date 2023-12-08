#include "data.h"
#include "aoc.h"

#include <numeric>

namespace AoC
{
    namespace Internal
    {
        bool IsGameSubsetInvalid(const GameSubset& subset, const GameSubset& max)
        {
            return subset.Red > max.Red || subset.Green > max.Green || subset.Blue > max.Blue;
        }

        GameSubset MaxGameSubSet(const GameSubset& currentMax, const GameSubset& subset)
        {
            return
            {
                std::max(currentMax.Red, subset.Red),
                std::max(currentMax.Green, subset.Green),
                std::max(currentMax.Blue, subset.Blue)
            };
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        std::string ignore{};
        std::string color{};
        u32 quantity{};
        while (std::getline(inputStream, line))
        {
            GameResult& result{ inputData.GameResults.emplace_back() };

            std::stringstream lineStream{ line };
            lineStream >> ignore >> result.GameId >> ignore;

            GameSubset* currentSubset{ &result.Subsets.emplace_back() };
            while (lineStream >> quantity >> color)
            {
                if (color.find("red") != std::string::npos)
                {
                    currentSubset->Red += quantity;
                }
                else if (color.find("green") != std::string::npos)
                {
                    currentSubset->Green += quantity;
                }
                else if (color.find("blue") != std::string::npos)
                {
                    currentSubset->Blue += quantity;
                }
                else
                {
                    return false;
                }

                if (color[color.size() - 1] == ';')
                {
                    currentSubset = &result.Subsets.emplace_back();
                }
            }
        }

        inputData.Max.Red = 12;
        inputData.Max.Green = 13;
        inputData.Max.Blue = 14;
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        output.SumValidGames = 0;
        output.SumMaxProduct = 0;
        for (const auto& game : input.GameResults)
        {
            bool isPossible{ std::all_of(game.Subsets.begin(), game.Subsets.end(),
                [&input](const auto& subset) { return !Internal::IsGameSubsetInvalid(subset, input.Max); }) };

            GameSubset maxValues{ std::accumulate(game.Subsets.begin(), game.Subsets.end(), GameSubset{}, Internal::MaxGameSubSet) };
            output.SumMaxProduct += maxValues.Red * maxValues.Green * maxValues.Blue;

            if (isPossible)
            {
                output.SumValidGames += game.GameId;
            }
        }
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.SumValidGames == 8;
        didTestsPass &= output.SumMaxProduct == 2286;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Sum Valid Game IDs: {}\n", output.SumValidGames);
        fmt::print("Sum Max Product: {}\n", output.SumMaxProduct);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData });
}
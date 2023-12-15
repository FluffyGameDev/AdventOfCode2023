#include "data.h"
#include "aoc.h"

#include <numeric>

namespace AoC
{
    namespace Internal
    {
        struct LensData
        {
            std::string_view Label;
            u8 FocalPower;
        };

        u8 ComputeHash(const std::string_view& command)
        {
            u8 hash{};

            for (char c : command)
            {
                hash += (u8)c;
                hash *= 17;
            }

            return hash;
        }

        void ParseCommand(const std::string_view& command, LensData& affectedLens, char& lensOperation)
        {
            size_t operationPos{};
            if ((operationPos = command.find_first_of('=')) != std::string_view::npos)
            {
                lensOperation = '=';
                affectedLens.Label = std::string_view(command.data(), operationPos);
                affectedLens.FocalPower = (u8)(command[operationPos + 1] - '0');
            }
            else if ((operationPos = command.find_first_of('-')) != std::string_view::npos)
            {
                lensOperation = '-';
                affectedLens.Label = std::string_view(command.data(), operationPos);
            }
            else
            {
                lensOperation = '\0';
                fmt::print("Invalid Command: '{}'.\n", command);
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        inputStream >> inputData.RawCommands;
        std::string_view commands{ inputData.RawCommands };

        size_t commaPosition{};
        while ((commaPosition = commands.find_first_of(',')) != std::string_view::npos)
        {
            inputData.Commands.emplace_back(commands.data(), commaPosition);
            commands.remove_prefix(commaPosition + 1);
        }
        inputData.Commands.emplace_back(commands);

        return true;
    };

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        auto accumulateHashes{ [](u64 total, std::string_view command) { return total + Internal::ComputeHash(command); } };
        output.SumHashes = std::accumulate(input.Commands.begin(), input.Commands.end(), 0ULL, accumulateHashes);

        std::vector<Internal::LensData> boxes[256];
        Internal::LensData affectedLens{};
        char lensOperation{};
        for (const std::string_view& command : input.Commands)
        {
            Internal::ParseCommand(command, affectedLens, lensOperation);

            u8 boxHash{ Internal::ComputeHash(affectedLens.Label) };
            std::vector<Internal::LensData>& affectedBox{ boxes[boxHash] };
            switch (lensOperation)
            {
                case '=':
                {
                    auto findPredicate{ [&affectedLens](const Internal::LensData& lens) { return affectedLens.Label == lens.Label; } };
                    auto foundIt{ std::find_if(affectedBox.begin(), affectedBox.end(), findPredicate) };
                    if (foundIt != affectedBox.end())
                    {
                        foundIt->FocalPower = affectedLens.FocalPower;
                    }
                    else
                    {
                        affectedBox.push_back(affectedLens);
                    }
                    break;
                }
                case '-':
                {
                    auto removePredicate{ [&affectedLens](const Internal::LensData& lens) { return affectedLens.Label == lens.Label; } };
                    auto removeIt{ std::remove_if(affectedBox.begin(), affectedBox.end(), removePredicate) };
                    if (removeIt != affectedBox.end())
                    {
                        affectedBox.erase(removeIt);
                    }
                    break;
                }
            }
        }

        for (u64 boxIndex = 0; boxIndex < 256; ++boxIndex)
        {
            std::vector<Internal::LensData>& box{ boxes[boxIndex] };
            for (u64 slotIndex = 0; slotIndex < box.size(); ++slotIndex)
            {
                u64 focusPower{ (boxIndex + 1) * (slotIndex + 1) * (u64)box[slotIndex].FocalPower };
                output.TotalFocusingPower += focusPower;
            }
        }
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.SumHashes == 1320;
        didTestsPass &= output.TotalFocusingPower == 145;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Sum of all hashes: {}\n", output.SumHashes);
        fmt::print("Total Focusing Power: {}\n", output.TotalFocusingPower);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData });
}
#include "data.h"
#include "aoc.h"

namespace AoC
{
    namespace Internal
    {
        void ComputeSequence(std::vector<s32>& nextSequence, const std::vector<s32>& previousSequence)
        {
            size_t newSequenceSize{ previousSequence.size() - 1 };
            nextSequence.reserve(newSequenceSize + 1);
            for (size_t i = 0; i < newSequenceSize; ++i)
            {
                nextSequence.push_back(previousSequence[i + 1] - previousSequence[i]);
            }
        }

        bool IsWholeSequenceZero(const std::vector<s32>& sequence)
        {
            return std::all_of(sequence.begin(), sequence.end(), [](s32 value) { return value == 0; });
        }

        void ComputePrediction(const std::vector<s32>& history, s32& prediction, s32& pastValue)
        {
            // Could be improved by reducing the number of allocations by pooling sequences.
            std::vector<std::vector<s32>> sequences{};
            sequences.push_back(history);

            while (!IsWholeSequenceZero(sequences.back()))
            {
                std::vector<s32>& newSequence{ sequences.emplace_back() };
                ComputeSequence(newSequence, sequences[sequences.size() - 2]);
            }

            prediction = sequences.back().back();
            pastValue = sequences.back().front();
            for (auto it = sequences.rbegin(); it != sequences.rend(); ++it)
            {
                prediction = it->back() + prediction;
                pastValue = it->front() - pastValue;
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        constexpr size_t DEFAULT_HISTORY_SIZE{ 300 };
        constexpr size_t DEFAULT_HISTORY_LINE_SIZE{ 30 };

        std::string line{};
        inputData.HistoryLines.reserve(DEFAULT_HISTORY_SIZE);
        while (std::getline(inputStream, line))
        {
            s32 number{};
            std::stringstream lineStream{ line };
            std::vector<s32>& newHistory{ inputData.HistoryLines.emplace_back() };
            newHistory.reserve(DEFAULT_HISTORY_LINE_SIZE);
            while (lineStream >> number)
            {
                newHistory.push_back(number);
            }
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        output.SumPredictions = 0;
        for (const auto& history : input.HistoryLines)
        {
            s32 prediction{};
            s32 pastValue{};
            Internal::ComputePrediction(history, prediction, pastValue);
            output.SumPredictions += prediction;
            output.SumPastValues += pastValue;
        }
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.SumPredictions == 114;
        didTestsPass &= output.SumPastValues == 2;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Sum of Predictions: {}\n", output.SumPredictions);
        fmt::print("Sum of Past Values: {}\n", output.SumPastValues);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData });
}
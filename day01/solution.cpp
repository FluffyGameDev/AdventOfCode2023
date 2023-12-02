#include "data.h"
#include "aoc.h"

namespace AoC
{
    const std::string k_Digits[]
    {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        std::string line{};
        while (std::getline(inputStream, line))
        {
            inputData.Lines.push_back(line);
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        bool readTextDigits{ true };
        output.SumFirstLastDigits = 0;
        for (const std::string& line : input.Lines)
        {
            int firstDigit{};
            int lastDigit{};
            int firstIndex{ std::numeric_limits<int>::max() };
            int lastIndex{ std::numeric_limits<int>::min() };
            for (int i = 0; i < line.size(); ++i)
            {
                int currentDigit = -1;
                if (std::isdigit(line[i]))
                {
                    currentDigit = line[i] - '0';
                }
                else if (readTextDigits)
                {
                    for (int j = 0; j < 9; ++j)
                    {
                        const std::string& digitText{ k_Digits[j] };
                        if (line.compare(i, digitText.size(), digitText) == 0)
                        {
                            currentDigit = j + 1;
                            break;
                        }
                    }
                }

                if (currentDigit >= 0)
                {
                    if (i < firstIndex) { firstIndex = i; firstDigit = currentDigit; }
                    if (i > lastIndex) { lastIndex = i; lastDigit = currentDigit; }
                }
            }

            u64 value{ firstDigit * 10ULL + lastDigit };
            output.SumFirstLastDigits += value;
        }
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.SumFirstLastDigits == 281;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Sum: {}", output.SumFirstLastDigits);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
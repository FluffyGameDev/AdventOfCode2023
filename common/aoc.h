#pragma once

#include <fmt/core.h>
#include <fstream>
#include <istream>
#include <sstream>

namespace AoC
{
    enum class AoCStep
    {
        Test,
        RealData
    };

    template<class InputData, class OutputData>
    void Run(const char* testInputData)
    {
        std::istringstream testInputStream{ testInputData };

        InputData testInput{};
        OutputData testOutput{};

        AoC::ReadInput(testInputStream, testInput, AoCStep::Test);
        AoC::ComputeOutput(testInput, testOutput);

        if (AoC::ValidateTestOutput(testOutput))
        {
            static const char* inputFile{ "input.txt" };
            std::ifstream inputStream{ inputFile };

            InputData inputData{};
            OutputData outputData{};

            bool readSucceeded{ inputStream.is_open() };
            if (readSucceeded)
            {
                readSucceeded &= AoC::ReadInput(inputStream, inputData, AoCStep::RealData);
                inputStream.close();
            }

            if (readSucceeded)
            {
                AoC::ComputeOutput(inputData, outputData);
                AoC::PrintOutput(outputData);
            }
            else
            {
                fmt::print("Failed to open input file.\n");
            }
        }
        else
        {
            fmt::print("Tests have failed.\n");
        }
    }
}
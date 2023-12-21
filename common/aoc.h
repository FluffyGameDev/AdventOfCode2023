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

    struct AoCContext
    {
        AoCStep Step;
        u32 PartNumber;
    };

    template<class InputData, class OutputData>
    void Run(std::initializer_list<const char*> testInputData)
    {
        u32 part{ 1 };
        for (const char* testInputText : testInputData)
        {
            fmt::print("Part {} Start:\n", part);
            fmt::print("==============\n");
            std::istringstream testInputStream{ testInputText };

            InputData testInput{};
            OutputData testOutput{};

            AoCContext context{};
            context.PartNumber = part;
            context.Step = AoCStep::Test;
            AoC::ReadInput(testInputStream, testInput, context);
            AoC::ComputeOutput(testInput, testOutput, context);

            if (AoC::ValidateTestOutput(testOutput, context))
            {
                fmt::print("Test Succeeded.\n");
                static const char* inputFile{ "input.txt" };
                std::ifstream inputStream{ inputFile };

                InputData inputData{};
                OutputData outputData{};

                bool readSucceeded{ inputStream.is_open() };
                if (readSucceeded)
                {
                    context.Step = AoCStep::RealData;
                    readSucceeded &= AoC::ReadInput(inputStream, inputData, context);
                    inputStream.close();
                }

                if (readSucceeded)
                {
                    AoC::ComputeOutput(inputData, outputData, context);
                    AoC::PrintOutput(outputData);
                }
                else
                {
                    fmt::print("Failed to open input file.\n");
                }
            }
            else
            {
                AoC::PrintOutput(testOutput);
                fmt::print("Tests have failed.\n");
            }

            fmt::print("==============\n\n");
            ++part;
        }
    }
}
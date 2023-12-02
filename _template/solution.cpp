#include "data.h"
#include "aoc.h"

namespace AoC
{
    namespace Internal
    {
        // TODO
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        // TODO
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        // TODO
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= false; // TODO

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        // TODO
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
#include "data.h"
#include "aoc.h"

namespace AoC
{
    namespace Internal
    {
        void ExpandUniverse(std::vector<Vector2>& galaxies, Vector2& universeSize, s32 offsetPerEmptyLine)
        {
            std::vector<u32> galaxiesPerColumn(universeSize.x, 0);
            std::vector<u32> galaxiesPerRow(universeSize.y, 0);
            std::vector<s32> offsetPerColumn{};
            std::vector<s32> offsetPerRow{};

            for (const Vector2& galaxy : galaxies)
            {
                ++galaxiesPerColumn[galaxy.x];
                ++galaxiesPerRow[galaxy.y];
            }

            s32 currentXOffset{};
            for (s32 i = 0; i < universeSize.x; ++i)
            {
                if (galaxiesPerColumn[i] == 0)
                {
                    currentXOffset += offsetPerEmptyLine;
                }
                offsetPerColumn.push_back(currentXOffset);
            }

            s32 currentYOffset{};
            for (s32 i = 0; i < universeSize.y; ++i)
            {
                if (galaxiesPerRow[i] == 0)
                {
                    currentYOffset += offsetPerEmptyLine;
                }
                offsetPerRow.push_back(currentYOffset);
            }

            for (Vector2& galaxy : galaxies)
            {
                galaxy.x += offsetPerColumn[galaxy.x];
                galaxy.y += offsetPerRow[galaxy.y];
            }

            universeSize.x += currentXOffset;
            universeSize.y += currentYOffset;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        while (std::getline(inputStream, line))
        {
            inputData.UniverseSize.x = (s32)line.size();

            for (s32 i = 0; i < inputData.UniverseSize.x; ++i)
            {
                if (line[i] == '#')
                {
                    inputData.Galaxies.push_back({ i, inputData.UniverseSize.y });
                }
            }

            ++inputData.UniverseSize.y;
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        std::vector<Vector2> galaxies{ input.Galaxies };
        Vector2 universeSize{ input.UniverseSize };

        s32 offsetPerEmptyLine{};
        switch (context.PartNumber)
        {
            case 1: offsetPerEmptyLine = 1; break;
            case 2: offsetPerEmptyLine = 99; break;
            case 3: offsetPerEmptyLine = 999999; break;
        }

        Internal::ExpandUniverse(galaxies, universeSize, offsetPerEmptyLine);

        for (size_t galaxyIndex1 = 0; galaxyIndex1 < (galaxies.size() - 1); ++galaxyIndex1)
        {
            for (size_t galaxyIndex2 = galaxyIndex1 + 1; galaxyIndex2 < galaxies.size(); ++galaxyIndex2)
            {
                output.SumShortestPath += Vector2::ManhattanDistance(galaxies[galaxyIndex1], galaxies[galaxyIndex2]);
            }
        }
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        if (context.PartNumber == 1)
        {
            didTestsPass &= output.SumShortestPath == 374;
        }
        else if (context.PartNumber == 2)
        {
            didTestsPass &= output.SumShortestPath == 8410;
        }

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Sum of shortest paths: {}\n", output.SumShortestPath);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData, AoC::s_testInputData, AoC::s_testInputData });
}
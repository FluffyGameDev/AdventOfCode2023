#include "data.h"
#include "aoc.h"

namespace AoC
{
    namespace Internal
    {
        char GetCharAtIndex(u32 x, u32 y, const Grid& grid)
        {
            return grid.Buffer[x + y * (u64)grid.Size.x];
        }

        std::tuple<u32, bool> FindMiddleLine(const Grid& grid, u32 maxErrorCount)
        {
            for (u32 x = 1; x < (u32)grid.Size.x; ++x)
            {
                u32 errorCount{};
                u32 maxIndex{ std::min(grid.Size.x - x, x) };
                for (u32 y = 0; y < (u32)grid.Size.y; ++y)
                {
                    for (u32 i = 0; i < maxIndex; ++i)
                    {
                        if (GetCharAtIndex(x + i, y, grid) != GetCharAtIndex(x - i - 1, y, grid))
                        {
                            ++errorCount;

                            if (errorCount > maxErrorCount)
                            {
                                break;
                            }
                        }
                    }

                    if (errorCount > maxErrorCount)
                    {
                        break;
                    }
                }

                if (errorCount == maxErrorCount)
                {
                    return { x, false };
                }
            }

            for (u32 y = 1; y < (u32)grid.Size.y; ++y)
            {
                u32 errorCount{};
                u32 maxIndex{ std::min(grid.Size.y - y, y) };
                for (u32 x = 0; x < (u32)grid.Size.x; ++x)
                {
                    for (u32 i = 0; i < maxIndex; ++i)
                    {
                        if (GetCharAtIndex(x, y + i, grid) != GetCharAtIndex(x, y - i - 1, grid))
                        {
                            ++errorCount;

                            if (errorCount > maxErrorCount)
                            {
                                break;
                            }
                        }
                    }

                    if (errorCount > maxErrorCount)
                    {
                        break;
                    }
                }

                if (errorCount == maxErrorCount)
                {
                    return { y, true };
                }
            }

            return { 0U, false };
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        Grid* currentGrid{ &inputData.Grids.emplace_back() };
        while (std::getline(inputStream, line))
        {
            if (line.empty())
            {
                currentGrid = &inputData.Grids.emplace_back();
            }
            else
            {
                currentGrid->Size.x = (u32)line.size();
                ++currentGrid->Size.y;
                currentGrid->Buffer.insert(currentGrid->Buffer.end(), line.begin(), line.end());
            }
        }

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        u32 maxErrorCount{ context.PartNumber == 1 ? 0U : 1U };
        for (const Grid& grid : input.Grids)
        {
            auto [midLine, isVertical] { Internal::FindMiddleLine(grid, maxErrorCount) };
            output.MidRowColumnCount += isVertical ? midLine * 100 : midLine;
        }
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        if (context.PartNumber == 1)
        {
            didTestsPass &= output.MidRowColumnCount == 405;
        }
        else
        {
            didTestsPass &= output.MidRowColumnCount == 400;
        }

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Mid Row Column Count: {}\n", output.MidRowColumnCount);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData, AoC::s_testInputData });
}
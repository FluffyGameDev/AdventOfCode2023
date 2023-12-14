#include "data.h"
#include "aoc.h"

#include <unordered_map>

namespace AoC
{
    namespace Internal
    {
        size_t FlattenIndex(s32 x, s32 y, const Grid& grid)
        {
            return x + y * (size_t)grid.Size.x;
        }

        size_t FlattenIndex(const Vector2& position, const Grid& grid)
        {
            return position.x + position.y * (size_t)grid.Size.x;
        }

        bool IsInGrid(const Vector2& position, const Grid& grid)
        {
            return position.x >= 0 && position.y >= 0 && position.x < grid.Size.x && position.y < grid.Size.y;
        }

        void MoveInDirection(Vector2& position, const Vector2& direction, const Grid& grid)
        {
            while (IsInGrid(position + direction, grid) && grid.Buffer[FlattenIndex(position + direction, grid)] == '.')
            {
                position = position + direction;
            }
        }

        void TiltNorth(Grid& grid)
        {
            for (s32 y = 0; y < grid.Size.y; ++y)
            {
                for (s32 x = 0; x < grid.Size.x; ++x)
                {
                    char& currentCell{ grid.Buffer[FlattenIndex(x, y, grid)] };
                    if (currentCell == 'O')
                    {
                        currentCell = '.';
                        Vector2 endPosition{ x, y };
                        MoveInDirection(endPosition, { 0, -1 }, grid);
                        grid.Buffer[FlattenIndex(endPosition, grid)] = 'O';
                    }
                }
            }
        }

        void TiltSouth(Grid& grid)
        {
            for (s32 y = grid.Size.y - 1; y >= 0 ; --y)
            {
                for (s32 x = 0; x < grid.Size.x; ++x)
                {
                    char& currentCell{ grid.Buffer[FlattenIndex(x, y, grid)] };
                    if (currentCell == 'O')
                    {
                        currentCell = '.';
                        Vector2 endPosition{ x, y };
                        MoveInDirection(endPosition, { 0, 1 }, grid);
                        grid.Buffer[FlattenIndex(endPosition, grid)] = 'O';
                    }
                }
            }
        }

        void TiltWest(Grid& grid)
        {
            for (s32 x = 0; x < grid.Size.x; ++x)
            {
                for (s32 y = 0; y < grid.Size.y; ++y)
                {
                    char& currentCell{ grid.Buffer[FlattenIndex(x, y, grid)] };
                    if (currentCell == 'O')
                    {
                        currentCell = '.';
                        Vector2 endPosition{ x, y };
                        MoveInDirection(endPosition, { -1, 0 }, grid);
                        grid.Buffer[FlattenIndex(endPosition, grid)] = 'O';
                    }
                }
            }
        }

        void TiltEast(Grid& grid)
        {
            for (s32 x = grid.Size.x - 1; x >= 0; --x)
            {
                for (s32 y = 0; y < grid.Size.y; ++y)
                {
                    char& currentCell{ grid.Buffer[FlattenIndex(x, y, grid)] };
                    if (currentCell == 'O')
                    {
                        currentCell = '.';
                        Vector2 endPosition{ x, y };
                        MoveInDirection(endPosition, { 1, 0 }, grid);
                        grid.Buffer[FlattenIndex(endPosition, grid)] = 'O';
                    }
                }
            }
        }

        void DoSpinCycle(Grid& grid)
        {
            TiltNorth(grid);
            TiltWest(grid);
            TiltSouth(grid);
            TiltEast(grid);
        }

        u64 ComputeNorthLoad(const Grid& grid)
        {
            u64 totalLoad{};
            for (s32 i = 0; i < grid.Buffer.size(); ++i)
            {
                if (grid.Buffer[i] == 'O')
                {
                    totalLoad += (u64)grid.Size.y - (i / grid.Size.x);
                }
            }
            return totalLoad;
        }

        void FindStonePositions(const Grid& grid, std::vector<u16>& stones)
        {
            u16 index{};
            stones.clear();
            for (char c : grid.Buffer)
            {
                if (c == 'O')
                {
                    stones.push_back(index);
                }
                ++index;
            }
        }

        void RestoreStonePositions(Grid& grid, const std::vector<u16>& stones)
        {
            for (char& c : grid.Buffer)
            {
                if (c == 'O')
                {
                    c = '.';
                }
            }

            for (u16 index : stones)
            {
                grid.Buffer[index] = 'O';
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        while (std::getline(inputStream, line))
        {
            ++inputData.Map.Size.y;
            inputData.Map.Size.x = (s32)line.size();
            inputData.Map.Buffer.append(line);
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        Grid grid{ input.Map };
        if (context.PartNumber == 1)
        {
            Internal::TiltNorth(grid);
        }
        else
        {
            u64 loopStart{};
            const u64 maxCycles{ 1000000000 };
            std::unordered_map<std::string, u64> gridConfigurations{};
            std::string stonePositionsString{};
            std::vector<u16> stonePositions{};

            for (u64 i = 0; i < maxCycles; ++i)
            {
                Internal::FindStonePositions(grid, stonePositions);
                stonePositionsString.resize(stonePositions.size() * 2);
                memcpy_s(stonePositionsString.data(), stonePositionsString.size(), stonePositions.data(), stonePositions.size() * 2);

                auto fountIt{ gridConfigurations.find(stonePositionsString) };
                if (fountIt != gridConfigurations.end())
                {
                    loopStart = fountIt->second;
                    break;
                }
                else
                {
                    gridConfigurations[stonePositionsString] = i;
                }

                Internal::DoSpinCycle(grid);
            }

            u64 loopSize{ gridConfigurations.size() - loopStart };
            u64 endIndex{ (maxCycles - loopStart) % loopSize + loopStart };
            auto foundIt{ std::find_if(gridConfigurations.begin(), gridConfigurations.end(),
                [endIndex](const auto& it) { return it.second == endIndex; }) };
            if (foundIt != gridConfigurations.end())
            {
                stonePositions.resize(foundIt->first.size() / 2);
                memcpy_s(stonePositions.data(), stonePositions.size() * 2, foundIt->first.data(), foundIt->first.size());
                Internal::RestoreStonePositions(grid, stonePositions);
            }
        }
        output.NorthBeamLoad = Internal::ComputeNorthLoad(grid);
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };


        if (context.PartNumber == 1)
        {
            didTestsPass &= output.NorthBeamLoad == 136;
        }
        else
        {
            didTestsPass &= output.NorthBeamLoad == 64;
        }

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("North Beam Load: {}\n", output.NorthBeamLoad);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData, AoC::s_testInputData });
}
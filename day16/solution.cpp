#include "data.h"
#include "aoc.h"

#include <vector>

namespace AoC
{
    namespace Internal
    {
        struct Ray
        {
            Vector2 StartPosition;
            Vector2 Direction;
            u32 Length;
        };

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
            return position.x >= 0 && position.y >= 0 && position.x < grid.Size.x&& position.y < grid.Size.y;
        }

        bool CanMoveThroughCell(const Vector2& position, const Vector2& direction, const Grid& grid, char& cell)
        {
            cell = grid.Data[FlattenIndex(position, grid)];
            switch (cell)
            {
                case '.': return true;
                case '-': return direction.x != 0;
                case '|': return direction.y != 0;
            }
            return false;
        }

        void CastRay(Ray& ray, const Grid& grid, Vector2& endPosition, char& lastCell)
        {
            endPosition = ray.StartPosition;
            do
            {
                endPosition = endPosition + ray.Direction;
                ++ray.Length;
            }
            while (IsInGrid(endPosition, grid) && CanMoveThroughCell(endPosition, ray.Direction, grid, lastCell));
        }

        bool HasRayBeenCast(const std::vector<Ray>& raysHistory, const Ray& ray)
        {
            for (const Ray& pastRay : raysHistory)
            {
                if (pastRay.StartPosition == ray.StartPosition && pastRay.Direction == ray.Direction)
                {
                    return true;
                }
            }
            return false;
        }

        void TryAddPendingRay(const std::vector<Ray>& rays, std::vector<Ray>& pendingRays, const Ray& ray)
        {
            if (!HasRayBeenCast(rays, ray) && !HasRayBeenCast(pendingRays, ray))
            {
                pendingRays.push_back(ray);
            }
        }

        void DrawRaysOnGrid(const std::vector<Ray>& rays, Grid& grid)
        {
            for (const Ray& ray : rays)
            {
                u32 cellsLeft{ ray.Length };
                Vector2 position{ ray.StartPosition };
                while (cellsLeft > 0)
                {
                    if (IsInGrid(position, grid))
                    {
                        grid.Data[FlattenIndex(position, grid)] = '#';
                    }
                    position = position + ray.Direction;
                    --cellsLeft;
                }
            }
        }

        u32 ComputeEnergizedCellCount(Grid grid, const Ray& firstRay)
        {
            std::vector<Internal::Ray> pendingRays{};
            std::vector<Internal::Ray> rays{};

            pendingRays.push_back(firstRay);
            while (!pendingRays.empty())
            {
                Internal::Ray currentRay{ pendingRays.back() };
                pendingRays.pop_back();

                char endCell{};
                Vector2 endPosition{};
                Internal::CastRay(currentRay, grid, endPosition, endCell);
                switch (endCell)
                {
                case '/':
                {
                    if (currentRay.Direction.x != 0)
                    {
                        Vector2 newDirection{ Vector2::Rotate90DegClockwise(currentRay.Direction) };
                        TryAddPendingRay(rays, pendingRays, { endPosition, newDirection, 0 });
                    }
                    else
                    {
                        Vector2 newDirection{ Vector2::Rotate90DegCounterClockwise(currentRay.Direction) };
                        TryAddPendingRay(rays, pendingRays, { endPosition, newDirection, 0 });
                    }
                    break;
                }
                case '\\':
                {
                    if (currentRay.Direction.x != 0)
                    {
                        Vector2 newDirection{ Vector2::Rotate90DegCounterClockwise(currentRay.Direction) };
                        TryAddPendingRay(rays, pendingRays, { endPosition, newDirection, 0 });
                    }
                    else
                    {
                        Vector2 newDirection{ Vector2::Rotate90DegClockwise(currentRay.Direction) };
                        TryAddPendingRay(rays, pendingRays, { endPosition, newDirection, 0 });
                    }
                    break;
                }
                case '-':
                case '|':
                {
                    TryAddPendingRay(rays, pendingRays, { endPosition, Vector2::Rotate90DegClockwise(currentRay.Direction), 0 });
                    TryAddPendingRay(rays, pendingRays, { endPosition, Vector2::Rotate90DegCounterClockwise(currentRay.Direction), 0 });
                    break;
                }
                }

                rays.push_back(currentRay);
            }

            Internal::DrawRaysOnGrid(rays, grid);
            return (u32)std::count(grid.Data.begin(), grid.Data.end(), '#');
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        while (std::getline(inputStream, line))
        {
            inputData.Map.Size.x = (s32)line.size();
            ++inputData.Map.Size.y;
            inputData.Map.Data.append(line);
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        output.EngergizedCells = Internal::ComputeEnergizedCellCount(input.Map, { { -1, 0 }, { 1, 0 }, 0 });

        for (s32 x = 0; x < input.Map.Size.x; ++x)
        {
            u32 energizedCellsNorth{ Internal::ComputeEnergizedCellCount(input.Map, { { x, -1 }, { 0, 1 }, 0 }) };
            u32 energizedCellsSouth{ Internal::ComputeEnergizedCellCount(input.Map, { { x, input.Map.Size.y }, { 0, -1 }, 0 }) };

            output.BestEngergizedCells = std::max(output.BestEngergizedCells, std::max(energizedCellsNorth, energizedCellsSouth));
        }

        for (s32 y = 0; y < input.Map.Size.y; ++y)
        {
            u32 energizedCellsWest{ Internal::ComputeEnergizedCellCount(input.Map, { { -1, y }, { 1, 0 }, 0 }) };
            u32 energizedCellsEast{ Internal::ComputeEnergizedCellCount(input.Map, { { input.Map.Size.x, y }, { -1, 0 }, 0 }) };

            output.BestEngergizedCells = std::max(output.BestEngergizedCells, std::max(energizedCellsWest, energizedCellsEast));
        }
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.EngergizedCells == 46;
        didTestsPass &= output.BestEngergizedCells == 51;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Engergized Cells: {}\n", output.EngergizedCells);
        fmt::print("Best Engergized Cells: {}\n", output.BestEngergizedCells);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData });
}
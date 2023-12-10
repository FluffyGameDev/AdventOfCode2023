#include "data.h"
#include "aoc.h"

#include <iostream>
#include <stack>

namespace AoC
{
    namespace Internal
    {
        enum class CellType
        {
            None,
            Pipe,
            LeftOfPipe,
            RightOfPipe
        };

        Vector2 IndexToCoord(size_t index, const Vector2& gridSize)
        {
            return { (s32)(index % gridSize.x), (s32)(index / gridSize.x) };
        }

        size_t CoordToIndex(Vector2 coord, const Vector2& gridSize)
        {
            return (size_t)(coord.x + (size_t)coord.y * gridSize.x);
        }

        bool IsPositionInGrid(const Vector2& position, const Vector2& gridSize)
        {
            return (position.x >= 0 && position.y >= 0 && position.x < gridSize.x && position.y < gridSize.y);
        }

        DirectionFlags DirectionToFlag(const Vector2& direction)
        {
            if (direction.x > 0) return DirectionFlags::East;
            if (direction.x < 0) return DirectionFlags::West;
            if (direction.y > 0) return DirectionFlags::South;
            if (direction.y < 0) return DirectionFlags::North;
            return DirectionFlags::None;
        }

        DirectionFlags ComputeOppositeDirection(DirectionFlags direction)
        {
            switch (direction)
            {
                case AoC::DirectionFlags::North: return DirectionFlags::South;
                case AoC::DirectionFlags::West: return DirectionFlags::East;
                case AoC::DirectionFlags::South: return DirectionFlags::North;
                case AoC::DirectionFlags::East: return DirectionFlags::West;
            }
            return DirectionFlags::None;
        }

        bool CanMoveToPosition(Vector2 direction, const Vector2& currentPosition, const InputData& input)
        {
            Vector2 nextPosition{ currentPosition + direction };
            if (IsPositionInGrid(nextPosition, input.GridSize))
            {
                DirectionFlags currentPipe{ input.Grid[CoordToIndex(currentPosition, input.GridSize)] };
                DirectionFlags nextPipe{ input.Grid[CoordToIndex(nextPosition, input.GridSize)] };
                DirectionFlags currentMask{ DirectionToFlag(direction) };
                DirectionFlags nextMask{ ComputeOppositeDirection(currentMask) };
                return ((u8)currentPipe & (u32)currentMask) != 0 && ((u8)nextPipe & (u32)nextMask) != 0;
            }
            return false;
        }

        Vector2 FindNextPosition(Vector2 currentPosition, const Vector2& previousPosition, const InputData& input)
        {
            Vector2 direction{ previousPosition - currentPosition };
            for (u32 i = 0; i < 3; ++i)
            {
                direction = Vector2::Rotate90DegCounterClockwise(direction);
                if (CanMoveToPosition(direction, currentPosition, input))
                {
                    currentPosition = currentPosition + direction;
                    break;
                }
            }
            return currentPosition;
        }

        void MarkCell(std::vector<CellType>& cellTypes, std::stack<Vector2>& cellsToProgagate, CellType cellType, const Vector2& position, const InputData& input)
        {
            if (IsPositionInGrid(position, input.GridSize))
            {
                size_t cellIndex{ CoordToIndex(position, input.GridSize) };
                if (cellTypes[cellIndex] == CellType::None)
                {
                    cellTypes[cellIndex] = cellType;
                    cellsToProgagate.push(position);
                }
            }
        }

        void MarkPipeCell(std::vector<CellType>& cellTypes, std::stack<Vector2>& cellsToProgagate, const Vector2& currentPosition, const Vector2& previousPosition, const InputData& input)
        {
            size_t currentPositionIndex{ CoordToIndex(currentPosition, input.GridSize) };
            cellTypes[currentPositionIndex] = CellType::Pipe;

            Vector2 direction{ currentPosition - previousPosition };

            MarkCell(cellTypes, cellsToProgagate, CellType::LeftOfPipe,
                previousPosition + Vector2::Rotate90DegCounterClockwise(direction), input);
            MarkCell(cellTypes, cellsToProgagate, CellType::RightOfPipe,
                previousPosition + Vector2::Rotate90DegClockwise(direction), input);
            MarkCell(cellTypes, cellsToProgagate, CellType::LeftOfPipe,
                currentPosition + Vector2::Rotate90DegCounterClockwise(direction), input);
            MarkCell(cellTypes, cellsToProgagate, CellType::RightOfPipe,
                currentPosition + Vector2::Rotate90DegClockwise(direction), input);
        }

        void PropagateCells(std::vector<CellType>& cellTypes, std::stack<Vector2>& cellsToProgagate, const InputData& input)
        {
            while (!cellsToProgagate.empty())
            {
                Vector2 currentCell{ cellsToProgagate.top() };
                size_t currentCellIndex{ CoordToIndex(currentCell, input.GridSize) };
                CellType typeToPropagate{ cellTypes[currentCellIndex] };
                cellsToProgagate.pop();

                if (typeToPropagate != CellType::Pipe)
                {
                    Vector2 direction{ 0, 1 };
                    for (u32 i = 0; i < 4; ++i)
                    {
                        Vector2 nextCell{ currentCell + direction };
                        MarkCell(cellTypes, cellsToProgagate, typeToPropagate, nextCell, input);
                        direction = Vector2::Rotate90DegCounterClockwise(direction);
                    }
                }
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        while (std::getline(inputStream, line))
        {
            inputData.GridSize.x = (s32)line.size();
            ++inputData.GridSize.y;

            for (char c : line)
            {
                switch (c)
                {
                    case '.': inputData.Grid.push_back(DirectionFlags::None); break;
                    case 'S': inputData.Grid.push_back(DirectionFlags::All); break;
                    case '-': inputData.Grid.push_back(DirectionFlags::Horizontal); break;
                    case '|': inputData.Grid.push_back(DirectionFlags::Vertical); break;
                    case 'L': inputData.Grid.push_back(DirectionFlags::CornerNE); break;
                    case 'J': inputData.Grid.push_back(DirectionFlags::CornerNW); break;
                    case '7': inputData.Grid.push_back(DirectionFlags::CornerSW); break;
                    case 'F': inputData.Grid.push_back(DirectionFlags::CornerSE); break;
                    default: return false;
                }
            }
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        auto foundStart{ std::find(input.Grid.begin(), input.Grid.end(), DirectionFlags::All) };
        size_t startFlatIndex{ (size_t)std::distance(input.Grid.begin(), foundStart) };
        Vector2 startPosition{ Internal::IndexToCoord(startFlatIndex, input.GridSize) };
        Vector2 currentPosition{ startPosition };
        Vector2 previousPosition{ startPosition + Vector2{ 1, 0 } };

        std::stack<Vector2> cellsToProgagate{};
        std::vector<Internal::CellType> cellTypes(input.Grid.size());
        std::fill(cellTypes.begin(), cellTypes.end(), Internal::CellType::None);

        do
        {
            Vector2 tmp{ currentPosition };
            currentPosition = Internal::FindNextPosition(currentPosition, previousPosition, input);
            previousPosition = tmp;

            Internal::MarkPipeCell(cellTypes, cellsToProgagate, currentPosition, previousPosition, input);
        }
        while (currentPosition != startPosition);

        Internal::PropagateCells(cellTypes, cellsToProgagate, input);
        output.FurthestPointLoopDistance = (u32)std::count(cellTypes.begin(), cellTypes.end(), Internal::CellType::Pipe) / 2;

        output.LeftTileCount = (u32)std::count(cellTypes.begin(), cellTypes.end(), Internal::CellType::LeftOfPipe);
        output.RightTileCount = (u32)std::count(cellTypes.begin(), cellTypes.end(), Internal::CellType::RightOfPipe);
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        if (context.PartNumber == 1)
        {
            didTestsPass &= output.FurthestPointLoopDistance == 8;
        }
        else if (context.PartNumber == 2)
        {
            didTestsPass &= (output.LeftTileCount == 10 || output.RightTileCount == 10);
        }

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Furthest Point Loop Distance: {}\n", output.FurthestPointLoopDistance);
        fmt::print("Left Tile Count: {}\n", output.LeftTileCount);
        fmt::print("Right Tile Count: {}\n", output.RightTileCount);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputDataPart1, AoC::s_testInputDataPart2 });
}
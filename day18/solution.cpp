#include "data.h"
#include "aoc.h"

#include <stack>
#include "vector2.h"

namespace AoC
{
    namespace Internal
    {

        size_t FlattenIndex(const Vector2& position, const Vector2& size)
        {
            return position.x + position.y * (size_t)size.x;
        }

        void ComputeBounds(const std::vector<CommandData>& commands, Vector2& minPosition, Vector2& maxPosition)
        {
            Vector2 currentPosition{};
            for (const CommandData& command : commands)
            {
                switch (command.Direction)
                {
                    case 'U': currentPosition.y -= command.Distance; break;
                    case 'D': currentPosition.y += command.Distance; break;
                    case 'L': currentPosition.x -= command.Distance; break;
                    case 'R': currentPosition.x += command.Distance; break;
                }

                minPosition.x = std::min(minPosition.x, currentPosition.x);
                minPosition.y = std::min(minPosition.y, currentPosition.y);
                maxPosition.x = std::max(maxPosition.x, currentPosition.x);
                maxPosition.y = std::max(maxPosition.y, currentPosition.y);
            }
        }

        void DrawLines(const std::vector<CommandData>& commands, const Vector2& minPosition, const Vector2& size, std::string& grid)
        {
            Vector2 currentPosition{ -minPosition.x, -minPosition.y };
            for (const CommandData& command : commands)
            {
                Vector2 direction{};
                switch (command.Direction)
                {
                case 'U': direction.y = -1; break;
                case 'D': direction.y = 1; break;
                case 'L': direction.x = -1; break;
                case 'R': direction.x = 1; break;
                }

                for (s32 i = 0; i < command.Distance; ++i)
                {
                    currentPosition = currentPosition + direction;
                    grid[FlattenIndex(currentPosition, size)] = '#';
                }
            }
        }

        u64 ComputePoolVolume(std::string& grid, const Vector2& size, const Vector2& minPosition)
        {
            std::stack<Vector2> pendingDraws{};
            pendingDraws.push({ -minPosition.x + 1, -minPosition.y + 1 });

            while (!pendingDraws.empty())
            {
                Vector2 currentPosition{ pendingDraws.top() };
                pendingDraws.pop();

                size_t currentIndex{ FlattenIndex(currentPosition, size) };
                if (grid[currentIndex] != '#')
                {
                    grid[currentIndex] = '#';
                    pendingDraws.push(currentPosition + Vector2{ 1, 0 });
                    pendingDraws.push(currentPosition + Vector2{ -1, 0 });
                    pendingDraws.push(currentPosition + Vector2{ 0, 1 });
                    pendingDraws.push(currentPosition + Vector2{ 0, -1 });
                }
            }

            return std::count(grid.begin(), grid.end(), '#');
        }

        void PrintGrid(const Vector2& size, const std::string& grid)
        {
            system("cls");
            s32 index{};
            for (char c : grid)
            {
                ++index;
                putc(c, stdout);
                if (index % size.x == 0)
                {
                    putc('\n', stdout);
                }
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        while (std::getline(inputStream, line))
        {
            std::stringstream lineStream{ line };
            CommandData& newCommand{ inputData.Commands.emplace_back() };
            lineStream >> newCommand.Direction >> newCommand.Distance;
        }

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        Vector2 minPosition{};
        Vector2 maxPosition{};
        Internal::ComputeBounds(input.Commands, minPosition, maxPosition);
        Vector2 size{ maxPosition - minPosition + Vector2{ 1, 1 } };

        std::string grid = std::string((size_t)size.x * size.y, '.');
        Internal::DrawLines(input.Commands, minPosition, size, grid);

        output.PoolVolume = Internal::ComputePoolVolume(grid, size, minPosition);
        Internal::PrintGrid(size, grid);
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.PoolVolume == 62;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Pool Volume: {}\n", output.PoolVolume);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData });
}
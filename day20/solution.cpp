#include "data.h"
#include "aoc.h"

#include <chrono>
#include <queue>
#include <string_view>

namespace AoC
{
    namespace Internal
    {
        enum class PulseType
        {
            Low,
            High
        };

        struct ModuleState
        {
            std::unordered_map<u64, PulseType> LastPulsePerInput;
            PulseType FlipFlopState{};
        };

        struct ModuleExecutionCommand
        {
            u64 SourceModule;
            u64 DestinationModule;
            PulseType PulseType;
        };

        void BuildModules(const std::vector<ModuleDescriptor>& moduleDescriptors, std::vector<ModuleState>& moduleStates)
        {
            size_t moduleCount{ moduleDescriptors.size() };
            moduleStates.resize(moduleCount);

            for (size_t i = 0; i < moduleCount; ++i)
            {
                for (u64 moduleId : moduleDescriptors[i].Destinations)
                {
                    moduleStates[moduleId].LastPulsePerInput.emplace(i, PulseType::Low);
                }
            }
        }

        void PrintCommand(const ModuleExecutionCommand& command, const InputData& input)
        {
            std::string_view sourceName{ command.SourceModule != std::numeric_limits<u64>::max() ? input.Modules[command.SourceModule].Name : "button" };
            std::string_view destinationName{ input.Modules[command.DestinationModule].Name };
            std::string_view pulseName{ command.PulseType == PulseType::Low ? "Low" : "High" };
            fmt::print("{} {} -> {}\n", sourceName, pulseName, destinationName);
        }

        void BroadcastPulse(const InputData& input, u64 broadcasterId, std::vector<Internal::ModuleState>& moduleStates, std::queue<ModuleExecutionCommand>& pendingCommands, OutputData& output)
        {
            pendingCommands.push({ std::numeric_limits<u64>::max(), broadcasterId, Internal::PulseType::Low });
            while (!pendingCommands.empty())
            {
                ModuleExecutionCommand command{ pendingCommands.front() };
                pendingCommands.pop();

                switch (command.PulseType)
                {
                    case PulseType::Low: ++output.LowPulseCount; break;
                    case PulseType::High: ++output.HighPulseCount; break;
                }

                const ModuleDescriptor& moduleDesc{ input.Modules[command.DestinationModule] };
                ModuleState& moduleState{ moduleStates[command.DestinationModule] };

                if (command.SourceModule != std::numeric_limits<u64>::max())
                {
                    moduleState.LastPulsePerInput.at(command.SourceModule) = command.PulseType;
                }

                switch (moduleDesc.ModuleType)
                {
                    case ModuleType::Broadcaster:
                    {
                        for (u64 destination : moduleDesc.Destinations)
                        {
                            pendingCommands.push({ command.DestinationModule, destination, command.PulseType });
                        }
                        break;
                    }
                    case ModuleType::FlipFlop:
                    {
                        if (command.PulseType == PulseType::Low)
                        {
                            moduleState.FlipFlopState = (moduleState.FlipFlopState == PulseType::Low ? PulseType::High : PulseType::Low);
                            for (u64 destination : moduleDesc.Destinations)
                            {
                                pendingCommands.push({ command.DestinationModule, destination, moduleState.FlipFlopState });
                            }
                        }
                        break;
                    }
                    case ModuleType::Conjuction:
                    {
                        bool allHigh{ std::all_of(moduleState.LastPulsePerInput.begin(), moduleState.LastPulsePerInput.end(),
                            [](const auto& state) { return state.second == PulseType::High; }) };
                        PulseType pulseToSend{ allHigh ? PulseType::Low : PulseType::High };

                        for (u64 destination : moduleDesc.Destinations)
                        {
                            pendingCommands.push({ command.DestinationModule, destination, pulseToSend });
                        }
                        break;
                    }
                }
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line;
        std::string destination;
        inputData.Modules.reserve(64);

        while (std::getline(inputStream, line))
        {
            std::stringstream lineStream{ line };

            std::string moduleName{};
            u64 moduleIndex{};
            ModuleType moduleType{};

            lineStream >> moduleName;
            
            if (moduleName[0] == '&' || moduleName[0] == '%')
            {
                std::string realName{ moduleName.substr(1) };
                auto foundIt{ inputData.ModuleIds.find(realName) };
                if (foundIt != inputData.ModuleIds.end())
                {
                    moduleIndex = foundIt->second;
                }
                else
                {
                    moduleIndex = inputData.Modules.size();
                    inputData.Modules.emplace_back();
                    inputData.ModuleIds.emplace(realName, moduleIndex);
                }

                moduleType = (moduleName[0] == '&' ? ModuleType::Conjuction : ModuleType::FlipFlop);
            }
            else if (moduleName == "broadcaster")
            {
                moduleIndex = inputData.Modules.size();
                inputData.Modules.emplace_back();
                inputData.ModuleIds.emplace(moduleName, moduleIndex);
                moduleType = ModuleType::Broadcaster;
            }
            else
            {
                return false;
            }

            ModuleDescriptor& currentModule{ inputData.Modules[moduleIndex] };
            currentModule.ModuleType = moduleType;
            currentModule.Name = moduleName;
            lineStream.seekg(4, lineStream.cur);

            while (lineStream >> destination)
            {
                if (destination[destination.size() - 1] == ',')
                {
                    destination.resize(destination.size() - 1);
                }

                u64 destinationIndex{};
                auto foundIt{ inputData.ModuleIds.find(destination) };
                if (foundIt != inputData.ModuleIds.end())
                {
                    destinationIndex = foundIt->second;
                }
                else
                {
                    destinationIndex = inputData.Modules.size();
                    inputData.Modules.emplace_back();
                    inputData.ModuleIds.emplace(destination, destinationIndex);
                }

                currentModule.Destinations.push_back(destinationIndex);
            }
        }

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        std::vector<Internal::ModuleState> moduleStates;
        Internal::BuildModules(input.Modules, moduleStates);

        u64 broadcasterId{ input.ModuleIds.at("broadcaster") };
        std::queue<Internal::ModuleExecutionCommand> pendingCommands;

        for (u64 i = 0; i < 1000; ++i)
        {
            Internal::BroadcastPulse(input, broadcasterId, moduleStates, pendingCommands, output);
        }
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        if (context.PartNumber == 1)
        {
            didTestsPass &= output.LowPulseCount == 8000;
            didTestsPass &= output.HighPulseCount == 4000;
        }
        else if (context.PartNumber == 2)
        {
            didTestsPass &= output.LowPulseCount == 4250;
            didTestsPass &= output.HighPulseCount == 2750;
        }

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Low Pulse count: {}\n", output.LowPulseCount);
        fmt::print("High Pulse count: {}\n", output.HighPulseCount);
        fmt::print("Pulse counts product: {}\n", output.LowPulseCount * output.HighPulseCount);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData1, AoC::s_testInputData2 });
}
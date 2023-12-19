#include "data.h"
#include "aoc.h"

#include <numeric>
#include <queue>
#include <string_view>
#include "range.h"

namespace AoC
{
    namespace Internal
    {
        struct PartRange
        {
            RangeU64 RatingRanges[(size_t)PartRating::Count];
        };

        void ParseRule(std::string_view text, Rule& rule)
        {
            size_t conditionPosition{};

            switch (text[0])
            {
                case 'x': rule.AffectedPartRating = PartRating::X; break;
                case 'm': rule.AffectedPartRating = PartRating::M; break;
                case 'a': rule.AffectedPartRating = PartRating::A; break;
                case 's': rule.AffectedPartRating = PartRating::S; break;
            }
            text.remove_prefix(1);

            if (text[0] == '<')
            {
                rule.RuleType = RuleType::Lower;
            }
            else if (text[0] == '>')
            {
                rule.RuleType = RuleType::Greater;
            }
            text.remove_prefix(1);

            rule.ComparisonValue = ::atoll(text.data());

            text.remove_prefix(text.find_first_of(':') + 1);
            rule.SuccessDestination = text;
        }

        void ParseWorkflow(std::string_view text, Workflow& workflow, std::string& workflowName)
        {
            size_t nameSize{ text.find_first_of('{') };
            workflowName = text.substr(0, nameSize);

            text.remove_prefix(nameSize + 1);
            size_t ruleSize{};
            while ((ruleSize = text.find_first_of(",")) != std::string_view::npos)
            {
                Rule& newRule{ workflow.Rules.emplace_back() };
                ParseRule(text.substr(0, ruleSize), newRule);
                text.remove_prefix(ruleSize + 1);
            }

            text.remove_suffix(1);
            Rule& lastRule{ workflow.Rules.emplace_back() };
            lastRule.RuleType = RuleType::AlwaysTrue;
            lastRule.SuccessDestination = text;
        }

        void ParsePart(const std::string& text, Part& part)
        {
            std::stringstream textStream{ text };
            textStream.seekg(3, textStream.cur);
            textStream >> part.Ratings[(size_t)PartRating::X];
            textStream.seekg(3, textStream.cur);
            textStream >> part.Ratings[(size_t)PartRating::M];
            textStream.seekg(3, textStream.cur);
            textStream >> part.Ratings[(size_t)PartRating::A];
            textStream.seekg(3, textStream.cur);
            textStream >> part.Ratings[(size_t)PartRating::S];
        }

        void ApplyWorkflow(const Part& part, const Workflow& workflow, std::string& nextWorkflowName)
        {
            for (const Rule& rule : workflow.Rules)
            {
                switch (rule.RuleType)
                {
                    case RuleType::AlwaysTrue:
                    {
                        nextWorkflowName = rule.SuccessDestination;
                        return;
                    }
                    case RuleType::Lower:
                    {
                        if (part.Ratings[(size_t)rule.AffectedPartRating] < rule.ComparisonValue)
                        {
                            nextWorkflowName = rule.SuccessDestination;
                            return;
                        }
                        break;
                    }
                    case RuleType::Greater:
                    {
                        if (part.Ratings[(size_t)rule.AffectedPartRating] > rule.ComparisonValue)
                        {
                            nextWorkflowName = rule.SuccessDestination;
                            return;
                        }
                        break;
                    }
                }
            }
        }

        bool IsPartAccepted(const Part& part, const InputData& input)
        {
            std::string currentWorkflow{ "in" };
            while (currentWorkflow != "A" && currentWorkflow != "R")
            {
                const Workflow& workflow{ input.Workflows.at(currentWorkflow) };
                ApplyWorkflow(part, workflow, currentWorkflow);
            }

            return currentWorkflow == "A";
        }

        u64 ComputeTotalAcceptedPartRatings(const InputData& input)
        {
            u64 sumAcceptedPartRatings{};
            for (const Part& part : input.Parts)
            {
                if (IsPartAccepted(part, input))
                {
                    sumAcceptedPartRatings += std::accumulate(std::begin(part.Ratings), std::end(part.Ratings), 0ULL, std::plus{});
                }
            }
            return sumAcceptedPartRatings;
        }

        u64 ComputeDistinctCombinations(const PartRange& partRange)
        {
            u64 total{ 1 };
            for (const RangeU64& range : partRange.RatingRanges)
            {
                total *= (range.Max - range.Min + 1);
            }
            return total;
        }

        u64 ComputeTotalDistinctAcceptedCombinations(const InputData& input)
        {
            u64 totalDistinctAcceptedCombinations{};
            PartRange initialPartRange{};
            std::fill(std::begin(initialPartRange.RatingRanges), std::end(initialPartRange.RatingRanges), RangeU64{ 1ULL, 4000ULL });
            std::queue<std::pair<std::string, PartRange>> pendingRanges{};
            pendingRanges.push({ "in", initialPartRange });

            while (!pendingRanges.empty())
            {
                auto [workflowName, partRange] { pendingRanges.front() };
                pendingRanges.pop();
                if (workflowName == "A")
                {
                    totalDistinctAcceptedCombinations += ComputeDistinctCombinations(partRange);
                    continue;
                }
                else if (workflowName == "R")
                {
                    continue;
                }

                const Workflow& workflow{ input.Workflows.at(workflowName) };
                for (const Rule& rule : workflow.Rules)
                {
                    bool canStopRules{};
                    switch (rule.RuleType)
                    {
                        case RuleType::AlwaysTrue:
                        {
                            pendingRanges.push({ rule.SuccessDestination, partRange });
                            canStopRules = true;
                            break;
                        }
                        case RuleType::Lower:
                        {
                            size_t ratingIndex{ (size_t)rule.AffectedPartRating };
                            RangeU64& partRatingRange{ partRange.RatingRanges[ratingIndex] };
                            if (partRatingRange.Min < rule.ComparisonValue)
                            {
                                if (partRatingRange.Max < rule.ComparisonValue)
                                {
                                    pendingRanges.push({ rule.SuccessDestination, partRange });
                                    canStopRules = true;
                                }
                                else
                                {
                                    PartRange newPartRange{ partRange };
                                    RangeU64& newPartRatingRange{ newPartRange.RatingRanges[ratingIndex] };

                                    partRatingRange.Min = rule.ComparisonValue;
                                    newPartRatingRange.Max = rule.ComparisonValue - 1;
                                    pendingRanges.push({ rule.SuccessDestination, newPartRange });
                                }
                            }
                            break;
                        }
                        case RuleType::Greater:
                        {
                            size_t ratingIndex{ (size_t)rule.AffectedPartRating };
                            RangeU64& partRatingRange{ partRange.RatingRanges[ratingIndex] };
                            if (partRatingRange.Max > rule.ComparisonValue)
                            {

                                if (partRatingRange.Min > rule.ComparisonValue)
                                {
                                    pendingRanges.push({ rule.SuccessDestination, partRange });
                                    canStopRules = true;
                                }
                                else
                                {
                                    PartRange newPartRange{ partRange };
                                    RangeU64& newPartRatingRange{ newPartRange.RatingRanges[ratingIndex] };

                                    partRatingRange.Max = rule.ComparisonValue;
                                    newPartRatingRange.Min = rule.ComparisonValue + 1;
                                    pendingRanges.push({ rule.SuccessDestination, newPartRange });
                                }
                            }
                            break;
                        }
                    }

                    if (canStopRules)
                    {
                        break;
                    }
                }
            }

            return totalDistinctAcceptedCombinations;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, const AoCContext& context)
    {
        std::string line{};
        std::string workflowName{};
        while (std::getline(inputStream, line))
        {
            if (line.empty()) break;

            Workflow workflow{};
            Internal::ParseWorkflow(line, workflow, workflowName);
            inputData.Workflows[workflowName] = std::move(workflow);
        }

        while (std::getline(inputStream, line))
        {
            Internal::ParsePart(line, inputData.Parts.emplace_back());
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output, const AoCContext& context)
    {
        output.SumAcceptedPartRatings += Internal::ComputeTotalAcceptedPartRatings(input);
        output.TotalDistinctAcceptedCombinations += Internal::ComputeTotalDistinctAcceptedCombinations(input);
    }

    bool ValidateTestOutput(const OutputData& output, const AoCContext& context)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.SumAcceptedPartRatings == 19114;
        didTestsPass &= output.TotalDistinctAcceptedCombinations == 167409079868000;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Sum of Accepted Part Ratings: {}\n", output.SumAcceptedPartRatings);
        fmt::print("Total Distinct Accepted Combinations: {}\n", output.TotalDistinctAcceptedCombinations);
    }
}

int main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>({ AoC::s_testInputData });
}
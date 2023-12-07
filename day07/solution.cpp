#include "data.h"
#include "aoc.h"

#include <numeric>
#include "indexedcontainerwrapper.h"

namespace AoC
{
    namespace Internal
    {
        Card ConvertCharToCard(char c)
        {
            Card card{ Card::Unknown };
            switch (c)
            {
                case '2': card = Card::Two; break;
                case '3': card = Card::Three; break;
                case '4': card = Card::Four; break;
                case '5': card = Card::Five; break;
                case '6': card = Card::Six; break;
                case '7': card = Card::Seven; break;
                case '8': card = Card::Eight; break;
                case '9': card = Card::Nine; break;
                case 'T': card = Card::Ten; break;
                case 'J': card = Card::Jack; break;
                case 'Q': card = Card::Queen; break;
                case 'K': card = Card::King; break;
                case 'A': card = Card::As; break;
            }
            return card;
        }

        HandType ComputeHandType(const HandData& hand)
        {
            u32 cardCounts[(u32)Card::Count];
            std::fill(std::begin(cardCounts), std::end(cardCounts), 0);
            for (Card card : hand.Cards)
            {
                ++cardCounts[(u32)card];
            }

            u32 jokerCount{ cardCounts[(u32)Card::Joker] };
            cardCounts[(u32)Card::Joker] = 0;
            if (jokerCount > 0)
            {
                u32 maxCountIndex{ 1 };
                for (u32 i = 1; i < (u32)Card::Count; ++i)
                {
                    if (cardCounts[i] > cardCounts[maxCountIndex])
                    {
                        maxCountIndex = i;
                    }
                }

                cardCounts[maxCountIndex] += jokerCount;
            }

            u32 pairCount{};
            u32 threeCount{};
            for (u32 cardCount : cardCounts)
            {
                switch (cardCount)
                {
                    case 2: ++pairCount; break;
                    case 3: ++threeCount; break;
                    case 4: return HandType::FourOfAKind;
                    case 5: return HandType::FiveOfAKind;
                }
            }

            if (threeCount > 0)
            {
                if (pairCount > 0)
                {
                    return HandType::FullHouse;
                }
                else
                {
                    return HandType::ThreeOfAKind;
                }
            }
            else if (pairCount == 2)
            {
                return HandType::TwoPair;
            }
            else if (pairCount == 1)
            {
                return HandType::OnePair;
            }

            return HandType::HighCard;
        }

        u32 ConvertHandToScore(const HandData& hand)
        {
            u32 score{ (u32)ComputeHandType(hand) };
            for (Card card : hand.Cards)
            {
                score = (score << 4) | (u32)card;
            }

            return score;
        }

        bool CompareHands(const HandData& lhs, const HandData& rhs)
        {
            return ConvertHandToScore(lhs) < ConvertHandToScore(rhs);
        } 

        void ConvertJacksToJokers(std::vector<HandData>& hands)
        {
            for (HandData& hand : hands)
            {
                for (Card& card : hand.Cards)
                {
                    if (card == Card::Jack)
                    {
                        card = Card::Joker;
                    }
                }
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        std::string line{};
        std::string hand{};
        while (std::getline(inputStream, line))
        {
            std::stringstream lineStream{ line };
            HandData& newHand{ inputData.Hands.emplace_back() };

            lineStream >> hand >> newHand.BidAmount;
            std::transform(hand.begin(), hand.end(), newHand.Cards, Internal::ConvertCharToCard);
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        std::vector<HandData> hands{ input.Hands };
        indexed_container_wrapper<std::vector<HandData>, HandData> indexedHands{ hands };
        auto accumulateWinnings{ [](u32 total, std::pair<const HandData&, size_t> value)
            { return total + value.first.BidAmount * (u32)(value.second + 1); } };

        std::sort(hands.begin(), hands.end(), Internal::CompareHands);
        output.TotalWinnings = std::accumulate(indexedHands.begin(), indexedHands.end(), 0UL, accumulateWinnings);

        Internal::ConvertJacksToJokers(hands);
        std::sort(hands.begin(), hands.end(), Internal::CompareHands);
        output.TotalWinningsJoker = std::accumulate(indexedHands.begin(), indexedHands.end(), 0UL, accumulateWinnings);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.TotalWinnings == 6440;
        didTestsPass &= output.TotalWinningsJoker == 5905;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Total Winnings: {}\n", output.TotalWinnings);
        fmt::print("Total Winnings with Jokers: {}\n", output.TotalWinningsJoker);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
#pragma once

#include <vector>
#include "types.h"

namespace AoC
{
    const char* s_testInputData
    {
        "32T3K 765\n"
        "T55J5 684\n"
        "KK677 28\n"
        "KTJJT 220\n"
        "QQQJA 483"
    };

    enum class Card : u8
    {
        Joker,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace,

        Count,
        Unknown
    };

    enum class HandType
    {
        HighCard,
        OnePair,
        TwoPair,
        ThreeOfAKind,
        FullHouse,
        FourOfAKind,
        FiveOfAKind,
    };

    static constexpr u32 CARD_COUNT{ 5 };

    struct HandData
    {
        Card Cards[CARD_COUNT];
        u32 BidAmount{};
    };

    struct InputData
    {
        std::vector<HandData> Hands;
    };

    struct OutputData
    {
        u32 TotalWinnings{};
        u32 TotalWinningsJoker{};
    };
}
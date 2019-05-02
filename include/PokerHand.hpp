/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PokerHand.hpp
InversePalindrome.com
*/


#pragma once
#include <string>
#include <vector>
#include <array>
#include "CardContainer.hpp"


class PokerHand : public CardContainer
{
public:
    enum HandStrength { UndefinedHand, HighCard, Pair, TwoPair, ThreeOfAKind, Straight, Flush, FullHouse, FourOfAKind, StraightFlush, RoyalFlush };

    PokerHand(const std::vector<Card>& cards);
    PokerHand(const std::vector<Card>& communityCards, const std::vector<Card>& holeCards);

    HandStrength getHandStrength() const;

    void classifyHand();

    std::size_t findHighCard(std::size_t numOfCards) const;
    std::size_t findPair() const;
    std::size_t findTwoPair() const;
    std::size_t findThreeOfAKind() const;
    std::size_t findStraight() const;
    std::size_t findFlush() const;
    std::size_t findFullHouse() const;
    std::size_t findFourOfAKind() const;
    std::size_t findStraightFlush() const;
    std::size_t findRoyalFlush() const;

    virtual std::string toString() const override;
    virtual std::string toString(std::size_t rank) const;

    bool operator==(const PokerHand& otherHand) const;
    bool operator!=(const PokerHand& otherHand) const;
    bool operator<(const PokerHand& otherHand) const;
    bool operator>(const PokerHand& otherHand) const;
    bool operator<=(const PokerHand& otherHand) const;
    bool operator>=(const PokerHand& otherHand) const;

private:
    HandStrength handStrength;
    std::size_t kicker;
    std::array<std::size_t, 13> rankTotal;

    void initializeRankTotal();
};
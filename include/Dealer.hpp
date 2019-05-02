/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Dealer.hpp
InversePalindrome.com
*/


#pragma once
#include <vector>
#include "Deck.hpp"
#include "Card.hpp"
#include "PokerTable.hpp"
#include "PokerHand.hpp"


class Dealer
{
public:
    enum Street { UndefinedStreet, Preflop, Flop, Turn, River };

    Dealer(const PokerTable& pokerTable);
    Dealer(const PokerTable& pokerTable, const Deck& deck);

    PokerTable getPokerTable() const;
    Deck getDeck() const;
    CardContainer getCommunityCards() const;
    CardContainer getMuckedCards() const;
    Street getStreet() const;
    std::size_t getCurrentPosition() const;

    void setPokerTable(const PokerTable& pokerTable);
    void setDeck(const Deck& deck);
    void setStreet(Street street);
    void setCurrentPosition(std::size_t currentPosition);

    void startHand();
    void dealPreFlop(std::size_t cardsPerPlayer);
    void dealFlop();
    void dealTurn();
    void dealRiver();
    void endHand();

    void makeFold(std::size_t playerPosition);
    void makeCall(std::size_t playerPosition);
    void makeBet(std::size_t playerPosition, std::size_t chips);
    void updateGameStage();

    std::size_t minContribution() const;
    std::size_t maxContribution() const;

    bool roundEnded() const;

private:
    PokerTable pokerTable;
    Deck deck;
    CardContainer communityCards;
    CardContainer muckedCards;
    Street street;
    std::size_t currentPosition;
    std::size_t actionCount;

    void changePositions();
    void moveCurrentPosition();
    void transferChipsFromPlayerToPot(std::size_t playerPosition, std::size_t chips);
    void transferPotToWinner();
    void transferCardsToDeck();

    void postBlinds();
    void postAnte();

    std::vector<PokerHand> rankedPokerHands() const;
};
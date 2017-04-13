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
#include "CardContainer.hpp"
#include "PokerHand.hpp"


class Dealer
{
public:
	Dealer(const PokerTable& pokerTable);
	Dealer(const PokerTable& pokerTable, const Deck& deck);

	PokerTable getPokerTable() const;
	Deck getDeck() const;
	CardContainer getCommunityCards() const;
	CardContainer getMuckedCards() const;

	void setPokerTable(const PokerTable& pokerTable);
	void setDeck(const Deck& deck);

	void startHand();
	void dealPreFlop(std::size_t cardsPerPlayer);
	void dealFlop();
	void dealTurn();
	void dealRiver();
	void endHand();

	void transferChipsFromPlayerToPot(std::size_t playerPosition, std::size_t chips);
	void makeFold(std::size_t playerPosition);
	void makeCall(std::size_t playerPosition);
	void makeBet(std::size_t playerPosition, std::size_t chips);
	
private:
	PokerTable pokerTable;
	Deck deck;
	CardContainer communityCards;
	CardContainer muckedCards;

	void changePositions();
	void transferPotToWinner();
	void transferCardsToDeck();

	void postBlinds();
	void postAnte();

	std::size_t minContribution() const;
	std::size_t maxContribution() const;
	std::vector<PokerHand> rankedPokerHands() const;
};
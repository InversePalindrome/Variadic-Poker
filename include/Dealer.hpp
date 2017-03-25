/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Dealer.hpp
InversePalindrome.com
*/


#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "Player.hpp"
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
	
private:
	PokerTable pokerTable;
	Deck deck;
	CardContainer communityCards;
	CardContainer muckedCards;

	void transferChipsFromPlayerToPot(std::size_t playerPosition, std::size_t chips);
	void transferPotToWinner();
	void transferCardsToDeck();

	void postBlinds();
	void muckHoleCards(Player& player);

	std::size_t minContribution() const;
	std::vector<PokerHand> rankedPokerHands() const;
};
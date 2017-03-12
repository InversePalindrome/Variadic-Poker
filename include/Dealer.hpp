/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Dealer.hpp
InversePalindrome.com
*/


#pragma once
#include <vector>
#include <string>
#include "Player.hpp"
#include "Deck.hpp"
#include "Card.hpp"
#include "PokerTable.hpp"
#include "HoleCards.hpp"


class Dealer
{
public:
	Dealer(const PokerTable& pokerTable);
	Dealer(const PokerTable& pokerTable, const Deck& deck);

	PokerTable getPokerTable() const;
	Deck getDeck() const;
	std::vector<Card> getBoard() const;
	std::vector<Card> getMuckedCards() const;

	void setPokerTable(const PokerTable& pokerTable);
	void setDeck(const Deck& deck);

	void dealPreFlop(std::size_t cardsPerPlayer);
	void dealFlop();
	void dealTurn();
	void dealRiver();
	void endHand();

	void transferChipsFromPlayerToPot(std::size_t playerPosition, std::size_t chips);

	std::string boardToString() const;

private:
	PokerTable pokerTable;
	Deck deck;
	std::vector<Card> board;
	std::vector<Card> muckedCards;
};
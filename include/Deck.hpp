/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Deck.hpp
InversePalindrome.com
*/


#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include "Card.hpp"


class Deck
{
public:
	Deck();
	Deck(const std::vector<Card>& deck);

	std::vector<Card> getDeck() const;
	std::size_t getSize() const;
	std::vector<Card> getCards(std::size_t numOfCards) const;
	Card getCard() const;

	void setDeck(const std::vector<Card>& deck);

	void shuffle();
	void removeCard(const Card& card);
	void removeCards(std::size_t numOfCards);
	void clearDeck();
	void addCard(const Card& card);
	void addCards(const std::vector<Card>& cards);

	bool hasCards() const;

private:
	std::vector<Card> deck;
};


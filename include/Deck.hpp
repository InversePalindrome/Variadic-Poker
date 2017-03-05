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
	Card dealCard();

	void setDeck(const std::vector<Card>& deck);

	void shuffle();
	void removeCard();
	void removeCard(const Card& card);
	void addCard(const Card& card);

private:
	std::vector<Card> deck;
};


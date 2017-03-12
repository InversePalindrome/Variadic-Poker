/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - HoleCards.hpp
InversePalindrome.com
*/


#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include "Card.hpp"


class HoleCards
{
public:
	HoleCards();
	HoleCards(const std::vector<Card>& holeCards);

	std::vector<Card> getCards() const;

	void setCards(const std::vector<Card>& holeCards);

	void addCard(const Card& card);
	void removeCard(const Card& card);
	void removeCards();

	bool hasCards() const;

	std::string toString() const;

private:
	std::vector<Card> holeCards;
};


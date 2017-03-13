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
#include "CardContainer.hpp"


class Deck : public CardContainer
{
public:
	Deck();
	Deck(const std::vector<Card>& deck);

	std::vector<Card> getCards(std::size_t numOfCards) const;
	Card getCard() const;

	void shuffle();
	void removeCards(std::size_t numOfCards);
};

/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Deck.hpp
InversePalindrome.com
*/


#pragma once
#include <list>
#include <algorithm>
#include "Card.hpp"


class Deck
{
public:
	Deck();
	Deck(const std::list<Card>& deck);

	std::list<Card> getDeck() const;
	
	void setDeck(const std::list<Card>& deck);

	void shuffle() const;
	Card removeCard() const;
	Card removeCard(const Card& card) const;
	void addCard(const Card& card) const;

private:
	std::list<Card> deck;
};
/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - CardContainer.hpp
InversePalindrome.com
*/


#pragma once
#include <vector>
#include <string>
#include "Card.hpp"


class CardContainer
{
public:
	CardContainer();
	CardContainer(const std::vector<Card>& cards);

	std::vector<Card> getCards() const;
	std::size_t getSize() const;

	void setCards(const std::vector<Card>& cards);

	void addCard(const Card& card);
	void addCards(const std::vector<Card>& cards);

	void removeCard(const Card& card);

	void clearCards();

	void sort();

	bool hasCards() const;

	virtual std::string toString() const;

protected:
	std::vector<Card> cards;
};
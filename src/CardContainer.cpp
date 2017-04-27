/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - CardContainer.cpp
InversePalindrome.com
*/


#include <algorithm>
#include "CardContainer.hpp"


CardContainer::CardContainer() :
	cards()
{
}

CardContainer::CardContainer(const std::vector<Card>& cards) :
	cards(cards)
{
}

std::vector<Card> CardContainer::getCards() const
{
	return this->cards;
}

std::size_t CardContainer::getSize() const
{
	return this->cards.size();
}

void CardContainer::setCards(const std::vector<Card>& cards)
{
	this->cards = cards;
}

void CardContainer::addCard(const Card& card)
{
	this->cards.push_back(card);
}

void CardContainer::addCards(const std::vector<Card>& cards)
{
	this->cards.insert(this->cards.end(), cards.begin(), cards.end());
}

void CardContainer::removeCard(const Card& card)
{
	this->cards.erase(std::remove(this->cards.begin(), this->cards.end(), card), this->cards.end());
}

void CardContainer::removeCards(const std::vector<Card>& cards)
{
	for (const auto& card : cards)
	{
		this->cards.erase(std::remove(this->cards.begin(), this->cards.end(), card), this->cards.end());
	}
}

void CardContainer::removeCards(std::size_t numOfCards)
{
	this->cards.resize(this->getSize() - numOfCards);
}

void CardContainer::clearCards()
{
	this->cards.clear();
}

void CardContainer::sort()
{
	std::sort(this->cards.rbegin(), this->cards.rend()); 
}

bool CardContainer::hasCards() const
{
	return !this->cards.empty();
}

std::string CardContainer::toString() const
{
	std::string cardRepresentation = "| ";
	for (const auto& card : this->cards)
	{
		cardRepresentation += card.toString() + " | ";
	}

	return cardRepresentation;
}
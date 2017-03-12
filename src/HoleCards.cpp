/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - HoleCards.cpp
InversePalindrome.com
*/


#include "HoleCards.hpp"


HoleCards::HoleCards() :
	holeCards()
{
}

HoleCards::HoleCards(const std::vector<Card>& holeCards) :
	holeCards(holeCards)
{
}

std::vector<Card> HoleCards::getCards() const
{
	return this->holeCards;
}

void HoleCards::setCards(const std::vector<Card>& holeCards)
{
	this->holeCards = holeCards;
}

void HoleCards::addCard(const Card& card)
{
	this->holeCards.push_back(card);
}

void HoleCards::removeCard(const Card& card)
{
	this->holeCards.erase(std::remove(this->holeCards.begin(), this->holeCards.end(), card), this->holeCards.end());
}

void HoleCards::removeCards()
{
	this->holeCards.clear();
}

bool HoleCards::hasCards() const
{
	return !this->holeCards.empty();
}

std::string HoleCards::toString() const
{
	std::string holeCardRepresentation = "| ";
	for (const auto& card : this->holeCards)
	{
		holeCardRepresentation += card.toString() + " | ";
	}

	return holeCardRepresentation;
}


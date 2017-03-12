/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Deck.cpp
InversePalindrome.com
*/


#include "Deck.hpp"


Deck::Deck() :
	deck()
{
	for (int suit = Card::Spade; suit <= Card::Heart; suit++)
	{
		for (int rank = Card::Two; rank <= Card::Ace; rank++)
		{
			deck.push_back(Card((Card::Suit)suit, (Card::Rank)rank));
		}
	}
}

Deck::Deck(const std::vector<Card>& deck) :
	deck(deck)
{
}

std::vector<Card> Deck::getDeck() const
{
	return this->deck;
}

std::vector<Card> Deck::getCards(std::size_t numOfCards) const
{
	if (numOfCards > 0 && numOfCards <= this->deck.size())
	{
		return std::vector<Card>(this->deck.end() - numOfCards , this->deck.end());
	}

	return {};
}

Card Deck::getCard() const
{
	return this->deck.back();
}

std::size_t Deck::getSize() const
{
	return this->deck.size();
}

void Deck::setDeck(const std::vector<Card>& deck)
{
	this->deck = deck;
}

void Deck::shuffle()
{
	std::shuffle(this->deck.begin(), this->deck.end(), std::default_random_engine{});
}

void Deck::removeCard(const Card& card)
{
	this->deck.erase(std::remove(this->deck.begin(), this->deck.end(), card), this->deck.end());
}

void Deck::removeCards(std::size_t numOfCards)
{
	this->deck.resize(this->deck.size() - numOfCards);
}

void Deck::clearDeck()
{
	this->deck.clear();
}

void Deck::addCard(const Card& card)
{
	this->deck.push_back(card);
}

void Deck::addCards(const std::vector<Card>& cards)
{
	this->deck.insert(this->deck.begin(), cards.begin(), cards.end());
}

bool Deck::hasCards() const
{
	return !this->deck.empty();
}


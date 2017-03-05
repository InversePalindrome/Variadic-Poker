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

std::size_t Deck::getSize() const
{
	return this->deck.size();
}

Card Deck::dealCard() 
{
	Card card = this->deck.back();
	this->removeCard();
	return this->deck.back();
}

void Deck::setDeck(const std::vector<Card>& deck)
{
	this->deck = deck;
}

void Deck::shuffle() 
{
	auto engine = std::default_random_engine{};
	std::shuffle(this->deck.begin(), this->deck.end(), engine);
}

void Deck::removeCard() 
{
	this->deck.pop_back();
}

void Deck::removeCard(const Card& card) 
{
	this->deck.erase(std::remove(this->deck.begin(), this->deck.end(), card), this->deck.end());
}

void Deck::addCard(const Card& card) 
{
	this->deck.push_back(card);
}
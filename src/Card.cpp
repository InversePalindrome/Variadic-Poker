/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Card.cpp
InversePalindrome.com
*/


#include "Card.hpp"


Card::Card() :
	suit(Suit::UndefinedSuit),
	rank(Rank::UndefinedRank)
{
}

Card::Card(Suit suit, Rank rank) :
	suit(suit),
	rank(rank)
{
}

Card::Suit Card::getSuit() const
{
	return this->suit;
}

Card::Rank Card::getRank() const
{
	return this->rank;
}

void Card::setSuit(Suit suit)
{
	this->suit = suit;
}

void Card::setRank(Rank rank)
{
	this->rank = rank;
}

std::string Card::toString() const
{
	std::string cardRank;

	switch (this->rank)
	{
	case Two:
		cardRank = "Two";
		break;
	case Three:
		cardRank = "Three";
		break;
	case Four:
		cardRank = "Four";
		break;
	case Five:
		cardRank = "Five";
		break;
	case Six:
		cardRank = "Six";
		break;
	case Seven:
		cardRank = "Seven";
		break;
	case Eight:
		cardRank = "Eight";
		break;
	case Nine:
		cardRank = "Nine";
		break;
	case Ten:
		cardRank = "Ten";
		break;
	case Jack:
		cardRank = "Jack";
		break;
	case Queen:
		cardRank = "Queen";
		break;
	case King:
		cardRank = "King";
		break;
	case Ace:
		cardRank = "Ace";
		break;
	}

	std::string cardSuit;

	switch (this->suit)
	{
	case Spade:
		cardSuit = "Spades";
		break;
	case Club:
		cardSuit = "Clubs";
		break;
	case Diamond:
		cardSuit = "Diamonds";
		break;
	case Heart:
		cardSuit = "Hearts";
		break;
	}

	return cardRank + " of " + cardSuit;
}

bool Card::operator==(const Card& otherCard) const
{
	return this->rank == otherCard.rank && this->suit == otherCard.suit;
}

bool Card::operator!=(const Card& otherCard) const
{
	return !(*this == otherCard);
}

bool Card::operator<(const Card& otherCard) const
{
	return this->rank < otherCard.rank;
}

bool Card::operator>(const Card& otherCard) const
{
	return !(*this < otherCard);
}
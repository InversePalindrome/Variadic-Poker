/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Card.hpp
InversePalindrome.com
*/


#pragma once
#include <string>


class Card
{
public:
	enum Suit { UndefinedSuit, Spade, Club, Diamond, Heart };
	enum Rank { UndefinedRank, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

	static const std::size_t SUIT_LENGTH = 4;
	static const std::size_t RANK_LENGTH = 13;

	Card();
	Card(Suit suit, Rank rank);

	Suit getSuit() const;
	Rank getRank() const;

	void setSuit(Suit suit);
	void setRank(Rank rank);

	std::string toString() const;

	bool operator==(const Card& otherCard) const;
	bool operator!=(const Card& otherCard) const;
	bool operator<(const Card& otherCard) const;
	bool operator>(const Card& otherCard) const;
	bool operator<=(const Card& otherCard) const;
	bool operator>=(const Card& otherCard) const;

private:
	Suit suit;
	Rank rank;
};
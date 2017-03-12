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
	enum Suit { Spade, Club, Diamond, Heart };
	enum Rank { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

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

private:
	Suit suit;
	Rank rank;
};

/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Player.hpp
InversePalindrome.com
*/


#pragma once
#include <string>
#include <cmath>
#include <stdexcept>
#include "HoleCards.hpp"


class Player
{
public:
	Player(const std::string& name, double stack);
	Player(const std::string & name);

	std::string getName() const;
	double getStack() const;
	HoleCards getHoleCards() const;

	void setName(const std::string& name);
	void setStack(double stack);
	void setHoleCards(const HoleCards& holeCards);

	void addToStack(double chips);
	void removeFromStack(double chips);

	std::string toString() const;

private:
	std::string name;
	double stack;
	HoleCards holeCards;
};
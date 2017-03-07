/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Player.cpp
InversePalindrome.com
*/


#include "Player.hpp"


Player::Player(const std::string& name, double stack) :
	name(name),
	stack(stack),
	holeCards()
{
}

Player::Player(const std::string& name) :
	name(name),
	stack(1000),
	holeCards()
{
}

std::string Player::getName() const
{
	return this->name;
}

double Player::getStack() const
{
	return this->stack;
}

HoleCards Player::getHoleCards() const 
{
	return this->holeCards;
}

void Player::setName(const std::string& name)
{
	this->name = name;
}

void Player::setStack(double stack)
{
	this->stack = fabs(stack);
}

void Player::setHoleCards(const HoleCards& holeCards)
{
	this->holeCards = holeCards;
}

void Player::addToStack(double chips)
{
	this->stack += fabs(chips);
}

void Player::removeFromStack(double chips)
{
	if (fabs(chips) > this->stack)
	{
		throw std::invalid_argument("The amount to be removed can't be more than the stack!");
	}
	else
	{
		this->stack -= fabs(chips);
	}
}

std::string Player::toString() const
{
	return this->name + " | " + std::to_string(this->stack) + " chips " + this->holeCards.toString();
}
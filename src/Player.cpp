/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Player.cpp
InversePalindrome.com
*/


#include "Player.hpp"


Player::Player(const std::string& name, std::size_t stack) :
	name(name),
	stack(stack),
	holeCards(),
	playingStatus(true)
{
}

Player::Player(const std::string& name) :
	name(name),
	stack(1000),
	holeCards(),
	playingStatus(true)
{
}

std::string Player::getName() const
{
	return this->name;
}

std::size_t Player::getStack() const
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

void Player::setStack(std::size_t stack)
{
	this->stack = stack;
}

void Player::setHoleCards(const HoleCards& holeCards)
{
	this->holeCards = holeCards;
}

void Player::setPlayingStatus(bool playingStatus)
{
	this->playingStatus = playingStatus;
}

void Player::addToStack(std::size_t chips)
{
	this->stack += chips;
}

void Player::removeFromStack(std::size_t chips)
{
	if (chips > this->stack)
	{
		throw std::invalid_argument("The amount to be removed can't be more than the stack!");
	}
	else
	{
		this->stack -= chips;
	}
}

void Player::removeAllStack()
{
	this->stack = 0;
}

void Player::removeHoleCards()
{
	this->holeCards.removeCards();
}

bool Player::isPlaying() const
{
	return this->playingStatus;
}

std::string Player::toString() const
{
	return "| " + this->name + " | " + std::to_string(this->stack) + " chips " + this->holeCards.toString();
}

bool Player::operator==(const Player& otherPlayer) const
{
	return this->name == otherPlayer.name;
}

bool Player::operator!=(const Player& otherPlayer) const
{
	return !(*this == otherPlayer);
}


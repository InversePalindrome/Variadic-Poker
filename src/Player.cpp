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
	action(PlayerAction::Action::UndefinedAction)
{
}

Player::Player(const std::string& name) :
	name(name),
	stack(1000),
	holeCards(),
	action(PlayerAction::Action::UndefinedAction)
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

CardContainer Player::getHoleCards() const
{
	return this->holeCards;
}

PlayerAction Player::getAction() const
{
	return this->action;
}

void Player::setName(const std::string& name)
{
	this->name = name;
}

void Player::setStack(std::size_t stack)
{
	this->stack = stack;
}

void Player::setHoleCards(const CardContainer& holeCards)
{
	this->holeCards = holeCards;
}

void Player::setAction(PlayerAction action)
{
	this->action = action;
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
	this->holeCards.clearCards();
}

bool Player::hasFolded() const
{
	return this->action.getAction() == PlayerAction::Action::Fold;
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

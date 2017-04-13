/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Player.cpp
InversePalindrome.com
*/


#include "Player.hpp"


Player::Player(const std::string& name) :
	Player(name, 1000)
{
}

Player::Player(const std::string& name, std::size_t stack) :
	name(name),
	stack(stack),
	potContribution(0),
	holeCards(),
	action(Action::UndefinedAction)
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

std::size_t Player::getPotContribution() const
{
	return this->potContribution;
}

CardContainer Player::getHoleCards() const
{
	return this->holeCards;
}

Player::Action Player::getAction() const
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

void Player::setPotContribution(std::size_t potContribution)
{
	this->potContribution = potContribution;
}

void Player::setHoleCards(const CardContainer& holeCards)
{
	this->holeCards = holeCards;
}

void Player::setAction(Action action)
{
	this->action = action;
}

void Player::addToStack(std::size_t chips)
{
	this->stack += chips;
}

void Player::addToPotContribution(std::size_t chips)
{
	chips > this->stack ? this->potContribution += this->stack : this->potContribution += chips;
}

void Player::removeFromStack(std::size_t chips)
{
	chips > this->stack ? this->stack = 0 : this->stack -= chips;
}

void Player::removeFromPotContribution(std::size_t chips)
{
	chips > this->potContribution ? this->potContribution = 0 : this->potContribution -= chips;
}

void Player::clearStack()
{
	this->stack = 0;
}

void Player::clearPotContribution()
{
	this->potContribution = 0;
}

void Player::clearHoleCards()
{
	this->holeCards.clearCards();
}

bool Player::isActive() const
{
	return this->action != Action::Fold;
}

std::string Player::toString() const
{
	return this->name + "\n" + std::to_string(this->stack) + " chips";
}

bool Player::operator==(const Player& otherPlayer) const
{
	return this->name == otherPlayer.name;
}

bool Player::operator!=(const Player& otherPlayer) const
{
	return !(*this == otherPlayer);
}
/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Player.hpp
InversePalindrome.com
*/


#pragma once
#include <string>
#include <cmath>
#include <stdexcept>
#include "CardContainer.hpp"
#include "PlayerAction.hpp"


class Player
{
public:
	Player(const std::string& name, std::size_t stack);
	Player(const std::string& name);

	std::string getName() const;
	std::size_t getStack() const;
	CardContainer getHoleCards() const;
	PlayerAction getAction() const;

	void setName(const std::string& name);
	void setStack(std::size_t stack);
	void setHoleCards(const CardContainer& holeCards);
	void setAction(PlayerAction action);

	void addToStack(std::size_t chips);
	void removeFromStack(std::size_t chips);
	void removeAllStack();
	void removeHoleCards();

	bool hasFolded() const;

	std::string toString() const;

	bool operator==(const Player& otherPlayer) const;
	bool operator!=(const Player& otherPlayer) const;

private:
	std::string name;
	std::size_t stack;
	CardContainer holeCards;
	PlayerAction action;
};
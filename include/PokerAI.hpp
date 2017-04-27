/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PokerAI.hpp
InversePalindrome.com
*/


#pragma once
#include "Dealer.hpp"
#include "Player.hpp"


class PokerAI
{
public:
	PokerAI(Dealer& dealer);

	void executeAction(std::size_t playerPosition);

private:
	Dealer& dealer ;

	double calculateRateOfReturn(std::size_t playerPosition) const;
	double calculatePotOdds(std::size_t playerPosition) const;
	double calculateHandStrength(std::size_t playerPosition) const;

	void selectAction(std::size_t playerPositionn, Player::Action playerAction);
};
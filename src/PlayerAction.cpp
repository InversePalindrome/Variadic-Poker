/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PlayerAction.cpp
InversePalindrome.com
*/


#include "PlayerAction.hpp"


PlayerAction::PlayerAction() :
	action(Action::UndefinedAction)
{
}

PlayerAction::PlayerAction(Action action) :
	action(action)
{
}

PlayerAction::Action PlayerAction::getAction() const
{
	return this->action;
}

void PlayerAction::setAction(PlayerAction::Action action)
{
	this->action = action;
}

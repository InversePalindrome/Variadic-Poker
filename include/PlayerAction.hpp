/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PlayerAction.hpp
InversePalindrome.com
*/


#pragma once


class PlayerAction
{
public:
	enum Action {UndefinedAction, Fold, Check, Call, Bet, Raise};

	PlayerAction();
	PlayerAction(Action action);

	Action getAction() const;
	
	void setAction(Action action);

private:
	Action action;
};

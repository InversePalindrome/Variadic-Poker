/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - StateStack.cpp
InversePalindrome.com
*/


#include "StateStack.hpp"


StateStack::StateStack() :
	states(),
	pendingActions(),
	stateMap()
{
}

void StateStack::processEvent()
{
	if (!this->states.empty())
	{
		this->states.back()->processEvent();
	}

	this->applyPendingState();
}

void StateStack::update(sf::Time deltaTime)
{
	if (!this->states.empty())
	{
		this->states.back()->update(deltaTime);
	}

	this->applyPendingState();
}

void StateStack::draw()
{
	if (!this->states.empty())
	{
		this->states.back()->draw();
	}
}

void StateStack::pushState(StateID stateName)
{
	this->pendingActions.push_back(PendingAction(StackAction::Push, stateName));
}

void StateStack::popState()
{
	this->pendingActions.push_back(PendingAction(StackAction::Pop, StateID::UndefinedState));
}

void StateStack::clearStates()
{
	this->pendingActions.push_back(PendingAction(StackAction::Clear, StateID::UndefinedState));
}

bool StateStack::hasStates() const
{
	return !this->states.empty();
}

StateStack::PendingAction::PendingAction(StackAction stackAction, StateID stateID) :
	stackAction(stackAction),
	stateID(stateID)
{
}

std::unique_ptr<GameState> StateStack::selectState(StateID stateName)
{
	return this->stateMap.find(stateName)->second();
}

void StateStack::applyPendingState()
{
	for (const auto& action : this->pendingActions)
	{
		switch (action.stackAction)
		{
		case Push:
			this->states.push_back(this->selectState(action.stateID));
			break;
		case Pop:
			this->states.pop_back();
			break;
		case Clear:
			this->states.clear();
			break;
		}
	}
	
	this->pendingActions.clear();
}
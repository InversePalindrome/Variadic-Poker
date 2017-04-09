/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - StateStack.cpp
InversePalindrome.com
*/


#include "StateStack.hpp"


StateStack::StateStack() :
	sfgui(),
	states(),
	pendingActions(),
	stateMap()
{
}

void StateStack::processEvent()
{
	if (!this->states.empty())
	{
		this->states.top()->processEvent();
	}

	this->applyPendingState();
}

void StateStack::update(sf::Time deltaTime)
{
	if (!this->states.empty())
	{
		this->states.top()->update(deltaTime);
	}

	this->applyPendingState();
}

void StateStack::draw()
{
	if (!this->states.empty())
	{
		this->states.top()->draw();
	}
}

void StateStack::pushState(StateID stateName)
{
	this->pendingActions.push_back(std::make_pair(StackAction::Push, stateName));
}

void StateStack::popState()
{
	this->pendingActions.push_back(std::make_pair(StackAction::Pop, StateID::UndefinedState));
}

void StateStack::clearStates()
{
	this->pendingActions.push_back(std::make_pair(StackAction::Clear, StateID::UndefinedState));
}

bool StateStack::hasStates() const
{
	return !this->states.empty();
}

std::unique_ptr<GameState> StateStack::selectState(StateID stateName)
{
	return this->stateMap.find(stateName)->second();
}

void StateStack::applyPendingState()
{
	for (const auto& action : this->pendingActions)
	{
		switch (action.first)
		{
		case Push:
			this->states.push(this->selectState(action.second));
			break;
		case Pop:
			this->states.pop();
			break;
		case Clear:
			while (!this->states.empty())
			{
				this->states.pop();
			}

			break;
		}
	}
	
	this->pendingActions.clear();
}
/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - StateStack.hpp
InversePalindrome.com
*/


#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <SFML/System/Time.hpp>
#include <SFGUI/SFGUI.hpp>
#include "GameState.hpp"


class StateStack
{
public:
	enum StateID { UndefinedState, SplashState, MenuState, GameSelectState, PlayState };
	enum StackAction { UndefinedAction, Pop, Push, Clear };

	StateStack();

	sfg::SFGUI sfgui;

	void processEvent();
	void update(sf::Time deltaTime);
	void draw();

	template<typename T>
	void registerState(StateID stateName, StateStack& states, GameState::Data& data);

	void pushState(StateID stateName);

	void popState();

	void clearStates();

	bool hasStates() const;

private:
	std::vector<std::unique_ptr<GameState>> states;
	std::vector<std::pair<StackAction, StateID>> pendingActions;
	std::map<StateID, std::function<std::unique_ptr<GameState>()>> stateMap;

	std::unique_ptr<GameState> selectState(StateID stateName);
	void applyPendingState();
};


template<typename T>
void StateStack::registerState(StateID stateName, StateStack& states, GameState::Data& data)
{
	this->stateMap[stateName] = [&]() { return std::unique_ptr<GameState>(new T(states, data)); };
}
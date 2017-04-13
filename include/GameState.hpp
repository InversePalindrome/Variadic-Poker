/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - GameState.hpp
InversePalindrome.com
*/


#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFGUI/Desktop.hpp>
#include "TextureManager.hpp"
#include "PokerTable.hpp"


class StateStack;

class GameState
{
public:
	struct Data
	{
		Data(sf::RenderWindow& window, TextureManager& textures, PokerTable& pokerTable);

		sf::RenderWindow& window;
		TextureManager& textures;
		PokerTable& pokerTable;
	};

	GameState(StateStack& states, Data& data);

	virtual void processEvent() = 0;
	virtual void update(sf::Time deltaTime) = 0;
	virtual void draw() = 0;

protected:
	StateStack& states;
	Data data;
	sfg::Desktop hud;
};
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


class StateStack;

class GameState
{
public:
	GameState(sf::RenderWindow& window, StateStack& states, TextureManager& textures);

	virtual void processEvent() = 0;
	virtual void update(sf::Time deltaTime) = 0;
	virtual void draw() = 0;

protected:
	sf::RenderWindow& window;
	StateStack& states;
	TextureManager& textures;
	sfg::Desktop hud;
};
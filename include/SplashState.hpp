/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - SplashState.hpp
InversePalindrome.com
*/


#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "GameState.hpp"
#include "TextureManager.hpp"


class StateStack;

class SplashState : public GameState
{
public:
	SplashState(StateStack& states, Data& data);

	virtual void processEvent() override;
	virtual void update(sf::Time deltaTime) override;
	virtual void draw() override;

private:
	sf::Sprite background;
	sf::Clock timer;
};
/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - MenuState.hpp
InversePalindrome.com
*/


#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFGUI/Button.hpp>
#include "GameState.hpp"
#include "TextureManager.hpp"


class StateStack;

class MenuState : public GameState
{
public:
	MenuState(sf::RenderWindow& window, StateStack& states, TextureManager& textures);

	virtual void processEvent() override;
	virtual void update(sf::Time deltaTime) override;
	virtual void draw() override;

private:
	sf::Sprite background;
	sfg::Button::Ptr playButton;

	void transitionToGameSelect();
};
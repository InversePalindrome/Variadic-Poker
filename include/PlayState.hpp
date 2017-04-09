/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PlayState.hpp
InversePalindrome.com
*/


#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Entry.hpp>
#include "GameState.hpp"
#include "TextureManager.hpp"


class StateStack;

class PlayState : public GameState
{
public:
	PlayState(sf::RenderWindow& window, StateStack& states, TextureManager& textures);

	virtual void processEvent() override;
	virtual void update(sf::Time deltaTime) override;
	virtual void draw() override;

private:
	sf::Sprite background;
	sfg::Box::Ptr actionFrame;
	sfg::Scrollbar::Ptr betBar;
	sfg::Scale::Ptr betScale;
	sfg::Adjustment::Ptr betAdjustment;
	sfg::Button::Ptr menuButton;
	sfg::Button::Ptr foldButton;
	sfg::Button::Ptr callButton;
	sfg::Button::Ptr betButton;
	sfg::Entry::Ptr betEntry;

	void transitionToMenu();
};

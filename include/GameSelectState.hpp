/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - TableSelectState.hpp
InversePalindrome.com
*/


#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Label.hpp>
#include "GameState.hpp"
#include "TextureManager.hpp"


class StateStack;

class GameSelectState : public GameState
{
public:
	GameSelectState(sf::RenderWindow& window, StateStack& states, TextureManager& textures);

	virtual void processEvent() override;
	virtual void update(sf::Time deltaTime) override;
	virtual void draw() override;

private:
	sf::Sprite background;
	sfg::Window::Ptr selectionWindow;
	sfg::Box::Ptr selectionFrame;
	sfg::ComboBox::Ptr tableSizeOptions;
	sfg::Button::Ptr menuButton;
	sfg::Button::Ptr doneButton;
	sfg::Entry::Ptr playerNameEntry;
	sfg::Entry::Ptr bigBlindEntry;
	sfg::Entry::Ptr anteEntry;
	sfg::Entry::Ptr playerStackEntry;
	sfg::Label::Ptr playerNameLabel;
	sfg::Label::Ptr tableSizeLabel;
	sfg::Label::Ptr bigBlindLabel;
	sfg::Label::Ptr anteLabel;
	sfg::Label::Ptr playerStackLabel;

	void transitionToMenu();
	void transitionToPlay();
};

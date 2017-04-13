/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PlayState.hpp
InversePalindrome.com
*/


#pragma once
#include <vector>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Label.hpp>
#include "GameState.hpp"
#include "TextureManager.hpp"
#include "Dealer.hpp"


class StateStack;

class PlayState : public GameState
{
public:
	PlayState(StateStack& states, Data& data);

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
	sfg::Label::Ptr tableLabel;
	std::vector<sfg::Label::Ptr> playerLabels;

	Dealer dealer;
	const std::string mainPlayerName;

	void transitionToMenu();
	void positionPlayerLabels();
	void adjustBetEntry();
	void updatePlayerLabel(const std::string& playerName, std::size_t playerLabelPosition);
};

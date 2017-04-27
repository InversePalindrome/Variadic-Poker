/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - GameState.cpp
InversePalindrome.com
*/


#include "GameState.hpp"
#include "StateStack.hpp"


GameState::Data::Data(sf::RenderWindow& window, sfg::SFGUI& sfgui, TextureManager& textures, PokerTable& pokerTable) :
	window(window),
	sfgui(sfgui),
	textures(textures),
	pokerTable(pokerTable)
{
}

GameState::GameState(StateStack& states, Data& data) :
	states(states),
	data(data),
	hud()
{
}
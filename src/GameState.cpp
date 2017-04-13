/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - GameState.cpp
InversePalindrome.com
*/


#include "GameState.hpp"
#include "StateStack.hpp"


GameState::Data::Data(sf::RenderWindow& window, TextureManager& textures, PokerTable& pokerTable) :
	window(window),
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
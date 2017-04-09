/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - GameState.cpp
InversePalindrome.com
*/


#include "GameState.hpp"
#include "StateStack.hpp"


GameState::GameState(sf::RenderWindow& window, StateStack& states, TextureManager& textures) :
	window(window),
	states(states),
	textures(textures),
	hud()
{
}
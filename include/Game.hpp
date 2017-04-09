/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Game.hpp
InversePalindrome.com
*/


#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFGUI/SFGUI.hpp>
#include "StateStack.hpp"
#include "TextureManager.hpp"


class Game
{
public:
	Game();
	void run();

private:
	sf::RenderWindow window;
	StateStack states;
	TextureManager textures;
};

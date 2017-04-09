/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Game.cpp
InversePalindrome.com
*/


#include <memory>
#include <SFML/System/Time.hpp>
#include "Game.hpp"
#include "GameState.hpp"
#include "SplashState.hpp"
#include "MenuState.hpp"
#include "GameSelectState.hpp"
#include "PlayState.hpp"


Game::Game() :
	window(sf::VideoMode(1920, 1200), "Variadic-Poker"),
	states(),
	textures()
{
	states.registerState<SplashState>(StateStack::SplashState, window, states, textures);
	states.registerState<MenuState>(StateStack::MenuState, window, states, textures);
	states.registerState<GameSelectState>(StateStack::GameSelectState, window, states, textures);
	states.registerState<PlayState>(StateStack::PlayState, window, states, textures);

	states.pushState(StateStack::SplashState);
}

void Game::run()
{
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	sf::Clock clock;
	sf::Time elapsedTime = sf::Time::Zero;

	while (this->window.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		elapsedTime += deltaTime;

		while (elapsedTime > timePerFrame)
		{
			elapsedTime -= timePerFrame;

			this->states.processEvent();
			this->states.update(deltaTime);
			this->window.clear(sf::Color::Black);
		}

		this->states.draw();
		this->window.display();
	}
}
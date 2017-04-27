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
	states(),
	data(window, sfgui, textures, pokerTable),
	window(sf::VideoMode(1920, 1200), "Variadic-Poker", sf::Style::Titlebar | sf::Style::Close),
	sfgui(),
	textures(),
	pokerTable({})
{
	states.registerState<SplashState>(StateStack::SplashState, states, data);
	states.registerState<MenuState>(StateStack::MenuState, states, data);
	states.registerState<GameSelectState>(StateStack::GameSelectState, states, data);
	states.registerState<PlayState>(StateStack::PlayState, states, data);

	states.pushState(StateStack::SplashState);
}

void Game::run()
{
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	sf::Clock clock;
	sf::Time elapsedTime = sf::Time::Zero;

	while (this->window.isOpen())
	{
		this->states.processEvent();

		sf::Time deltaTime = clock.restart();
		elapsedTime += deltaTime;

		while (elapsedTime > timePerFrame)
		{
			elapsedTime -= timePerFrame;

			this->states.update(deltaTime);
			this->window.clear(sf::Color::Black);
		}

		this->states.draw();
		this->window.display();
	}
}
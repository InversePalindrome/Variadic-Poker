/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - SplashState.cpp
InversePalindrome.com
*/


#include <memory>
#include <SFML/Window/Event.hpp>
#include "StateStack.hpp"
#include "SplashState.hpp"
#include "MenuState.hpp"


SplashState::SplashState(sf::RenderWindow& window, StateStack& states, TextureManager& textures) :
	GameState(window, states, textures),
	background(),
	timer()
{
	textures.loadTexture("InPalLogo", "InversePalindromeLogo.png");
	const auto& logo = textures.getTexture("InPalLogo");

	background.setTexture(logo);
	background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);
	background.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
}

void SplashState::processEvent()
{
	sf::Event event;

	while (this->window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			this->window.close();
			break;
		}
	}

	if (this->timer.getElapsedTime().asSeconds() >= 3.f)
	{
		this->states.popState();
		this->states.pushState(StateStack::MenuState);
	}
}

void SplashState::update(sf::Time deltaTime)
{

}

void SplashState::draw()
{
	this->window.clear(sf::Color::White);
	this->window.draw(background);
}
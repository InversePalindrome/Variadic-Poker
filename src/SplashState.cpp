/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - SplashState.cpp
InversePalindrome.com
*/


#include <memory>
#include <SFML/Window/Event.hpp>
#include "SplashState.hpp"
#include "StateStack.hpp"
#include "MenuState.hpp"


SplashState::SplashState(StateStack& states, Data& data) :
	GameState(states, data),
	background(),
	timer()
{
	data.textures.loadTexture("InPalLogo", "InversePalindromeLogo.png");
	const auto& logo = data.textures.getTexture("InPalLogo");

	background.setTexture(logo);
	background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);
	background.setPosition(data.window.getSize().x / 2.f, data.window.getSize().y / 2.f);
}

void SplashState::processEvent()
{
	sf::Event event;

	while (this->data.window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			this->data.window.close();
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
	this->data.window.clear(sf::Color::White);
	this->data.window.draw(background);
}
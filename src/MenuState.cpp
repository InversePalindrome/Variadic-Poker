/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - MenuState.cpp
InversePalindrome.com
*/


#include <memory>
#include <SFML/Window/Event.hpp>
#include "StateStack.hpp"
#include "MenuState.hpp"
#include "GameSelectState.hpp"


MenuState::MenuState(sf::RenderWindow& window, StateStack& states, TextureManager& textures) :
	GameState(window, states, textures),
	background(),
	playButton()
{
	textures.loadTexture("MenuBackground", "MenuBackground.png");
	const auto& backgroundTexture = textures.getTexture("MenuBackground");
	
	background.setTexture(backgroundTexture);
	background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);
	background.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
	background.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x, static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

	playButton = sfg::Button::Create("   Play   ");
	playButton->SetPosition(sf::Vector2f(window.getSize().x / 2.15f, window.getSize().y / 1.1f));
	playButton->SetRequisition(sf::Vector2f(window.getSize().x / 40.f, window.getSize().y / 15.5f));
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect( [this] { transitionToGameSelect(); });
	 
	hud.Add(playButton);
    hud.SetProperty("*", "FontSize", 38.f);
}

void MenuState::processEvent()
{
	sf::Event event;

	while (this->window.pollEvent(event))
	{
		this->hud.HandleEvent(event);

		switch (event.type)
		{
		case sf::Event::Closed:
			this->window.close();
			break;
		case sf::Event::Resized:
			this->playButton->SetPosition(sf::Vector2f(this->window.getSize().x / 2.15f, this->window.getSize().y / 1.1f));
			break;
		}
	}
}

void MenuState::update(sf::Time time)
{
	this->playButton->Show(true);
	this->hud.Update(time.asMilliseconds());
}

void MenuState::draw()
{
	this->window.clear(sf::Color::Black);
	this->window.draw(this->background);
	this->states.sfgui.Display(this->window);
}

void MenuState::transitionToGameSelect()
{
	this->playButton->Show(false);
	this->states.pushState(StateStack::GameSelectState);
}
/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - MenuState.cpp
InversePalindrome.com
*/


#include <memory>
#include <SFML/Window/Event.hpp>
#include "MenuState.hpp"
#include "StateStack.hpp"
#include "GameSelectState.hpp"


MenuState::MenuState(StateStack& states, Data& data) :
	GameState(states, data),
	background(),
	playButton()
{
	data.textures.loadTexture("MenuBackground", "MenuBackground.png");
	const auto& backgroundTexture = data.textures.getTexture("MenuBackground");
	
	background.setTexture(backgroundTexture);
	background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);
	background.setPosition(data.window.getSize().x / 2.f, data.window.getSize().y / 2.f);
	background.setScale(static_cast<float>(data.window.getSize().x) / backgroundTexture.getSize().x, static_cast<float>(data.window.getSize().y) / backgroundTexture.getSize().y);

	playButton = sfg::Button::Create("   Play   ");
	playButton->SetPosition(sf::Vector2f(data.window.getSize().x / 2.15f, data.window.getSize().y / 1.1f));
	playButton->SetRequisition(sf::Vector2f(data.window.getSize().x / 40.f, data.window.getSize().y / 15.5f));
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect( [this] { transitionToGameSelect(); });

	hud.Add(playButton);
    hud.SetProperty("*", "FontSize", 38.f);
}

void MenuState::processEvent()
{
	sf::Event event;

	while (this->data.window.pollEvent(event))
	{
		this->hud.HandleEvent(event);

		switch (event.type)
		{
		case sf::Event::Closed:
			this->data.window.close();
			break;
		case sf::Event::Resized:
			this->playButton->SetPosition(sf::Vector2f(this->data.window.getSize().x / 2.15f, this->data.window.getSize().y / 1.1f));
			break;
		}
	}
}

void MenuState::update(sf::Time time)
{
	this->playButton->Show(true);
	this->hud.Update(static_cast<float>(time.asMicroseconds()));
}

void MenuState::draw()
{
	this->data.window.clear(sf::Color::Black);
	this->data.window.draw(this->background);
	this->states.sfgui.Display(this->data.window);
}

void MenuState::transitionToGameSelect()
{
	this->playButton->Show(false);
	this->states.pushState(StateStack::GameSelectState);
}
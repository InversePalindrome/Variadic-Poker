/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - TableSelectState.cpp
InversePalindrome.com
*/


#include <memory>
#include <SFML/Window/Event.hpp>
#include "StateStack.hpp"
#include "GameSelectState.hpp"
#include "MenuState.hpp"
#include "PlayState.hpp"


GameSelectState::GameSelectState(sf::RenderWindow& window, StateStack& states, TextureManager& textures) :
	GameState(window, states, textures),
	selectionWindow(),
	selectionFrame(),
	tableSizeOptions(),
	menuButton(),
	doneButton(),
	bigBlindEntry(),
	anteEntry(),
	playerNameEntry(),
	playerStackEntry(),
	tableSizeLabel(),
	bigBlindLabel(),
	anteLabel(),
	playerNameLabel(),
	playerStackLabel()
{
	textures.loadTexture("TableSelectBackground", "TableSelectBackground.jpg");
	const auto& backgroundTexture = textures.getTexture("TableSelectBackground");

	background.setTexture(backgroundTexture);
	background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);
	background.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
	background.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x, static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

	menuButton = sfg::Button::Create("  Menu  ");
	menuButton->GetSignal(sfg::Widget::OnLeftClick).Connect( [this] { transitionToMenu(); });

	doneButton = sfg::Button::Create("  Done  ");
	doneButton->GetSignal(sfg::Widget::OnLeftClick).Connect( [this] { transitionToPlay(); });

	tableSizeLabel = sfg::Label::Create("Table Size:");
	
	tableSizeOptions = sfg::ComboBox::Create();
	tableSizeOptions->AppendItem("Full-Ring");
	tableSizeOptions->AppendItem("6-Max");
	tableSizeOptions->AppendItem("Heads-Up");

	bigBlindLabel = sfg::Label::Create("Big Blind:");
	bigBlindEntry = sfg::Entry::Create();
	bigBlindEntry->SetRequisition(sf::Vector2f(80.f, 0.f));

	anteLabel = sfg::Label::Create("Ante:");
	anteEntry = sfg::Entry::Create();
	anteEntry->SetRequisition(sf::Vector2f(80.f, 0.f));

	playerNameLabel = sfg::Label::Create("Player Name:");
	playerNameEntry = sfg::Entry::Create();
	playerNameEntry->SetRequisition(sf::Vector2f(80.f, 0.f));

	playerStackLabel = sfg::Label::Create("  Starting Stack:  ");
	playerStackEntry = sfg::Entry::Create();
	playerStackEntry->SetRequisition(sf::Vector2f(80.f, 0.f));

	selectionFrame = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
	selectionFrame->Pack(playerNameLabel);
	selectionFrame->Pack(playerNameEntry);
	selectionFrame->Pack(tableSizeLabel);
	selectionFrame->Pack(tableSizeOptions);
	selectionFrame->Pack(bigBlindLabel);
	selectionFrame->Pack(bigBlindEntry);
	selectionFrame->Pack(anteLabel);
	selectionFrame->Pack(anteEntry);
	selectionFrame->Pack(playerStackLabel);
	selectionFrame->Pack(playerStackEntry);
	selectionFrame->Pack(doneButton);
	
	selectionWindow = sfg::Window::Create();
	selectionWindow->SetTitle("Game Selection");
	selectionWindow->Add(selectionFrame);
	selectionWindow->SetPosition(sf::Vector2f(window.getSize().x / 2.4f, window.getSize().y / 3.1f));

	hud.Add(menuButton);
	hud.Add(selectionWindow);
}

void GameSelectState::processEvent()
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
			this->selectionWindow->SetPosition(sf::Vector2f(window.getSize().x / 2.4f, window.getSize().y / 3.1f));
			break;
		}
	}
}

void GameSelectState::update(sf::Time deltaTime)
{
	this->menuButton->Show(true);
	this->selectionWindow->Show(true);
	this->hud.Update(deltaTime.asMilliseconds());
}

void GameSelectState::draw()
{
	this->window.clear(sf::Color::Black);
	this->window.draw(this->background);
	this->states.sfgui.Display(this->window);
}

void GameSelectState::transitionToMenu()
{
	this->menuButton->Show(false);
	this->selectionWindow->Show(false);
	this->states.popState();
}

void GameSelectState::transitionToPlay()
{
	this->menuButton->Show(false);
	this->selectionWindow->Show(false);
	this->states.popState();
	this->states.pushState(StateStack::PlayState);
}

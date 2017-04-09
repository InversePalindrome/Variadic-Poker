/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PlayState.cpp
InversePalindrome.com
*/


#include <SFML/Window/Event.hpp>
#include "StateStack.hpp"
#include "PlayState.hpp"


PlayState::PlayState(sf::RenderWindow& window, StateStack& states, TextureManager& textures) :
	GameState(window, states, textures),
	background(),
	actionFrame(),
	betBar(),
	betScale(),
	betAdjustment(),
	menuButton(),
	foldButton(),
	callButton(),
	betButton(),
	betEntry()
{
	textures.loadTexture("PokerTable", "PokerTable.png");
	const auto& backgroundTexture = textures.getTexture("PokerTable");

	background.setTexture(backgroundTexture);
	background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);
	background.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
	background.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x, static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

	betEntry = sfg::Entry::Create();
	betEntry->SetRequisition(sf::Vector2f(158.f, 0.f));

	betBar = sfg::Scrollbar::Create(sfg::Scrollbar::Orientation::HORIZONTAL);
	betBar->SetRequisition(sf::Vector2f(600.f, 40.f));

	betAdjustment = betBar->GetAdjustment();
	betAdjustment->SetLower(0.f);
	betAdjustment->SetUpper(100.f);
	betAdjustment->SetMinorStep(1.f);
	betAdjustment->SetMajorStep(1.f);
	betAdjustment->SetPageSize(8.f);
	
	betScale = sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL);
	betScale->SetAdjustment(betAdjustment);

	menuButton = sfg::Button::Create("  Menu  ");
	menuButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	foldButton = sfg::Button::Create("    Fold    ");

	callButton = sfg::Button::Create("    Call    ");

	betButton = sfg::Button::Create("    Bet    ");

	actionFrame = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 50.f);
	actionFrame->Pack(foldButton);
	actionFrame->Pack(callButton);
	actionFrame->Pack(betButton);
	actionFrame->Pack(betEntry);
	actionFrame->Pack(betBar);
	actionFrame->SetPosition(sf::Vector2f(220.f, window.getSize().y - 80.f));

	hud.Add(menuButton); 
	hud.Add(actionFrame);
}

void PlayState::processEvent()
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
			actionFrame->SetPosition(sf::Vector2f(220.f, window.getSize().y - 80.f));
			break;
		}
	}
}

void PlayState::update(sf::Time deltaTime)
{
	this->hud.Update(deltaTime.asMilliseconds());
}

void PlayState::draw()
{
	this->window.clear(sf::Color::Black);
	this->window.draw(this->background);
	this->states.sfgui.Display(this->window);
}

void PlayState::transitionToMenu()
{
	this->menuButton->Show(false);
	this->actionFrame->Show(false);
	this->states.popState();
}
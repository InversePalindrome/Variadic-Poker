/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PlayState.cpp
InversePalindrome.com
*/


#include <cmath>
#include <SFML/Window/Event.hpp>
#include "PlayState.hpp"
#include "StateStack.hpp"


PlayState::PlayState(StateStack& states, Data& data) :
	GameState(states, data),
	background(),
	actionFrame(),
	betBar(),
	betScale(),
	betAdjustment(),
	menuButton(),
	foldButton(),
	callButton(),
	betButton(),
	betEntry(),
	tableLabel(),
	playerLabels(),
	dealer(data.pokerTable),
	mainPlayerName(data.pokerTable.getPlayers().at(0).getName())
{
	data.textures.loadTexture("PokerTable", "PokerTable.png");
	const auto& backgroundTexture = data.textures.getTexture("PokerTable");
	
	background.setTexture(backgroundTexture);
	background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);
	background.setPosition(data.window.getSize().x / 2.f, data.window.getSize().y / 2.f);
	background.setScale(static_cast<float>(data.window.getSize().x) / backgroundTexture.getSize().x, static_cast<float>(data.window.getSize().y) / backgroundTexture.getSize().y);

	betEntry = sfg::Entry::Create();
	betEntry->SetRequisition(sf::Vector2f(180.f, 0.f));

	betBar = sfg::Scrollbar::Create(sfg::Scrollbar::Orientation::HORIZONTAL);
	betBar->SetRequisition(sf::Vector2f(600.f, 40.f));

	betAdjustment = betBar->GetAdjustment();
	betAdjustment->SetLower(0.f);
	betAdjustment->SetUpper(105.f);
	betAdjustment->SetMinorStep(1.f);
	betAdjustment->SetMajorStep(1.f);
	betAdjustment->SetPageSize(5.f);
	betAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect([this] { adjustBetEntry(); });
	
	betScale = sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL);
	betScale->SetAdjustment(betAdjustment);

	menuButton = sfg::Button::Create("  Menu  ");
	menuButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	foldButton = sfg::Button::Create("    Fold    ");
	foldButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, data] { dealer.makeFold(dealer.getPokerTable().findPlayer(data.pokerTable.getPlayer(mainPlayerName))); });

	callButton = sfg::Button::Create("    Call    ");
	callButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, data] { dealer.makeCall(dealer.getPokerTable().findPlayer(data.pokerTable.getPlayer(mainPlayerName))); 
	updatePlayerLabel(mainPlayerName, 0); });

	betButton = sfg::Button::Create("    Bet    ");
	betButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, data] { dealer.makeBet(dealer.getPokerTable().findPlayer(data.pokerTable.getPlayer(mainPlayerName)), static_cast<std::size_t>(std::stoull(betEntry->GetText().toAnsiString()))); 
	updatePlayerLabel(mainPlayerName, 0); });

	actionFrame = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 50.f);
	actionFrame->Pack(foldButton);
	actionFrame->Pack(callButton);
	actionFrame->Pack(betButton);
	actionFrame->Pack(betEntry);
	actionFrame->Pack(betBar);
	actionFrame->SetPosition(sf::Vector2f(data.window.getSize().x / 9.1f, data.window.getSize().y - 80.f));

	tableLabel = sfg::Label::Create(this->data.pokerTable.toString());
	tableLabel->SetPosition(sf::Vector2f(data.window.getSize().x / 2.5f, data.window.getSize().y / 4.f));

	const auto& players = dealer.getPokerTable().getPlayers();
	
	for (std::size_t i = 0; i < players.size(); i++)
	{
		playerLabels.push_back(sfg::Label::Create(players.at(i).toString()));
	}

	positionPlayerLabels();
	
	hud.Add(menuButton);
	hud.Add(tableLabel);
	hud.Add(actionFrame);

	for (const auto& playerLabel : playerLabels)
	{
		hud.Add(playerLabel);
	}
}

void PlayState::processEvent()
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
			this->actionFrame->SetPosition(sf::Vector2f(this->data.window.getSize().x / 9.1f, this->data.window.getSize().y - 80.f));
			this->tableLabel->SetPosition(sf::Vector2f(data.window.getSize().x / 2.8f, data.window.getSize().y / 4.f));
			this->positionPlayerLabels();
			break;
		}
	}
}

void PlayState::update(sf::Time deltaTime)
{
	this->hud.Update(static_cast<float>(deltaTime.asMicroseconds()));
}

void PlayState::draw()
{
	this->data.window.clear(sf::Color::Black);
	this->data.window.draw(this->background);
	this->states.sfgui.Display(this->data.window);
}

void PlayState::transitionToMenu()
{
	this->menuButton->Show(false);
	this->tableLabel->Show(false);
	this->actionFrame->Show(false);

	for (const auto& playerLabel : playerLabels)
	{
		playerLabel->Show(false);
	}

	this->data.pokerTable.clearPlayers();

	this->states.popState();
}

void PlayState::positionPlayerLabels()
{
	for (std::size_t i = 0; i < this->playerLabels.size(); i++)
	{
		const float PI = static_cast<float>(std::atan(1)) * 4;
		float xRadius = this->data.window.getSize().x / 2.5;
		float yRadius = this->data.window.getSize().y / 2.5;
		float theta = (2 * PI * i) / this->playerLabels.size() + (PI / 2);
		this->playerLabels.at(i)->SetPosition(sf::Vector2f(std::round(xRadius * std::cos(theta)) + this->data.window.getSize().x / 2.2, std::round(yRadius * std::sin(theta)) + this->data.window.getSize().y / 2.3));
	}
}

void PlayState::adjustBetEntry()
{
	this->betEntry->SetText(std::to_string(static_cast<std::size_t>(this->betAdjustment->GetValue() / (this->betAdjustment->GetUpper() - this->betAdjustment->GetPageSize()) * this->dealer.getPokerTable().getPlayer(this->mainPlayerName).getStack())));
}

void PlayState::updatePlayerLabel(const std::string& playerName, std::size_t playerLabelPosition)
{
	this->playerLabels.at(playerLabelPosition)->SetText(this->dealer.getPokerTable().getPlayer(playerName).toString());
}
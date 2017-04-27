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
	potLabel(),
	playerLabels(),
	cardSprites(),
	dealer(data.pokerTable),
	pokerAI(dealer),
	mainPlayerName(data.pokerTable.getPlayers().at(data.pokerTable.getSize() > 2 ? 2 : 0).getName()),
	mainPlayerActed(false)
{
	assignCardTextures();

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
		[this, data] { dealer.makeFold(dealer.getPokerTable().findPlayer(data.pokerTable.getPlayer(mainPlayerName)));
	actionFrame->Show(false);
	mainPlayerActed = true;
	dealer.updateGameStage();
	});
	
	callButton = sfg::Button::Create("    Call    ");
	callButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, data] { dealer.makeCall(dealer.getPokerTable().findPlayer(data.pokerTable.getPlayer(mainPlayerName))); 
	actionFrame->Show(false);
	mainPlayerActed = true;
	dealer.updateGameStage();
	});

	betButton = sfg::Button::Create("    Bet    ");
	betButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, data] { processBet();
	actionFrame->Show(false);
	mainPlayerActed = true;
	dealer.updateGameStage();
	});

	actionFrame = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 50.f);
	actionFrame->Pack(foldButton);
	actionFrame->Pack(callButton);
	actionFrame->Pack(betButton);
	actionFrame->Pack(betEntry);
	actionFrame->Pack(betBar);
	actionFrame->SetPosition(sf::Vector2f(data.window.getSize().x / 9.1f, data.window.getSize().y - 80.f));

	tableLabel = sfg::Label::Create(data.pokerTable.toString());
	tableLabel->SetPosition(sf::Vector2f(data.window.getSize().x / 2.5f, data.window.getSize().y / 4.f));

	dealer.updateGameStage();
	
	potLabel = sfg::Label::Create("| Pot: " + std::to_string(this->dealer.getPokerTable().getPot()) + " | Call Amount: " + std::to_string(this->dealer.maxContribution() - this->dealer.getPokerTable().getPlayer(this->mainPlayerName).getPotContribution()) + " |");
	potLabel->SetPosition(sf::Vector2f(data.window.getSize().x / 2.65f, data.window.getSize().y / 1.8f));

	const auto& players = dealer.getPokerTable().getPlayers();

	for (auto& player : players)
	{
		playerLabels.push_back(std::make_pair(player.getName(), sfg::Label::Create(player.toString())));
	}

	positionPlayerLabels();
	
	hud.Add(menuButton);
	hud.Add(tableLabel);
	hud.Add(potLabel);
	hud.Add(actionFrame);
	
	for (const auto& playerLabel : playerLabels)
	{
		hud.Add(playerLabel.second);
	}
}

void PlayState::processEvent()
{
	sf::Event event;

	this->mainPlayerActed = false;

	while (this->data.window.pollEvent(event))
	{
		this->hud.HandleEvent(event);
		
		switch (event.type)
		{
		case sf::Event::Closed:
			this->data.window.close();
			break;
		}
	}
	
	if (this->data.pokerTable.hasPlayers() && !this->mainPlayerActed)
	{
		this->executePokerGame();
	}

	this->updatePlayerLabels();
	this->updatePotLabel();
}

void PlayState::update(sf::Time deltaTime)
{
	this->hud.Update(static_cast<float>(deltaTime.asMicroseconds()));
}

void PlayState::draw()
{
	this->data.window.clear(sf::Color::Black);
	this->data.window.draw(this->background);
	this->drawCommunityCards();
	this->drawHoleCards(this->mainPlayerName);
	this->data.sfgui.Display(this->data.window);
}

void PlayState::executePokerGame()
{
	if (this->dealer.getPokerTable().getPlayers().at(this->dealer.getCurrentPosition()).getName() == this->mainPlayerName)
	{
		this->actionFrame->Show(true);
	}
	else 
	{
		this->pokerAI.executeAction(this->dealer.getCurrentPosition());
		this->dealer.updateGameStage();
	}
}

void PlayState::transitionToMenu()
{
	this->data.pokerTable.clearPlayers();
	this->menuButton->Show(false);
	this->tableLabel->Show(false);
	this->potLabel->Show(false);
	this->actionFrame->Show(false);

	for (const auto& playerLabel : playerLabels)
	{
		playerLabel.second->Show(false);
	}

	this->states.popState();
}

void PlayState::processBet()
{
	try
	{
		this->dealer.makeBet(this->dealer.getPokerTable().findPlayer(data.pokerTable.getPlayer(mainPlayerName)), static_cast<std::size_t>(std::stoull(betEntry->GetText().toAnsiString())));
	}
	catch (const std::invalid_argument& e)
	{
		this->dealer.makeCall(this->dealer.getPokerTable().findPlayer(data.pokerTable.getPlayer(mainPlayerName)));
	}
}

void PlayState::positionPlayerLabels()
{
	std::size_t labelPosition = this->playerLabels.size() > 2 ? 2 : 0;

	for (std::size_t i = 0; i < playerLabels.size(); i++, labelPosition++)
	{
		if (labelPosition == playerLabels.size())
		{
			labelPosition = 0;
		}

		const float PI = static_cast<float>(std::atan(1)) * 4;
		float xRadius = this->data.window.getSize().x / 2.5f;
		float yRadius = this->data.window.getSize().y / 2.5f;
		float theta = (2 * PI * i) / this->playerLabels.size() + (PI / 2);
		this->playerLabels.at(labelPosition).second->SetPosition(sf::Vector2f(std::round(xRadius * std::cos(theta)) + this->data.window.getSize().x / 2.2f, std::round(yRadius * std::sin(theta)) + this->data.window.getSize().y / 2.3f));
	}
}

void PlayState::adjustBetEntry()
{
	this->betEntry->SetText(std::to_string(static_cast<std::size_t>(this->betAdjustment->GetValue() / (this->betAdjustment->GetUpper() - this->betAdjustment->GetPageSize()) * this->dealer.getPokerTable().getPlayer(this->mainPlayerName).getStack())));
}

void PlayState::updatePlayerLabels()
{
	const auto& players = this->dealer.getPokerTable().getPlayers();

	for (auto& playerLabel : this->playerLabels)
	{
		auto player = std::find_if(players.begin(), players.end(),
			[&](const Player& player) { return player.getName() == playerLabel.first; });

		if (player != players.end())
		{
			playerLabel.second->SetText(player->toString());
		}
		else
		{
			playerLabel.second->Show(false);
		}
	}
}

void PlayState::updatePotLabel()
{
	potLabel->SetText("| Pot: " + std::to_string(this->dealer.getPokerTable().getPot()) + " | Call Amount: " + std::to_string(this->dealer.maxContribution() - this->dealer.getPokerTable().getPlayer(this->mainPlayerName).getPotContribution()) + " |");
}

void PlayState::assignCardTextures()
{
	const auto& cards = Deck().CardContainer::getCards();

	for (std::size_t i = 1; i <= 52; i++)
	{
		const auto& card = cards.at(i - 1).toString();

		if (!this->data.textures.hasTexture(card))
		{
			this->data.textures.loadTexture(card, "Card" + std::to_string(i) + ".png");
		}

		sf::Sprite cardSprite(this->data.textures.getTexture(card));
		cardSprite.setScale(0.28f, 0.3f);

		this->cardSprites.insert(std::make_pair(card, cardSprite));
	}
}

void PlayState::drawCommunityCards()
{
	if (this->dealer.getPokerTable().activePlayerCount() > 1)
	{
		const auto& communityCards = this->dealer.getCommunityCards().getCards();

		for (std::size_t i = 0; i < communityCards.size(); i++)
		{
			std::size_t xOffset = static_cast<std::size_t>(i * (this->data.window.getSize().x / 10.f));
			this->cardSprites.at(communityCards.at(i).toString()).setPosition(sf::Vector2f((this->data.window.getSize().x / 3.6f) + xOffset, this->data.window.getSize().y / 3.f));
			this->data.window.draw(this->cardSprites.at(communityCards.at(i).toString()));
		}
	}
}

void PlayState::drawHoleCards(const std::string& playerName)
{
	const auto& playerLabel = std::find_if(this->playerLabels.begin(), this->playerLabels.end(),
		[&](const std::pair<std::string, sfg::Label::Ptr> playerLabel) { return playerLabel.first == playerName; });

	const auto& holeCards = this->dealer.getPokerTable().getPlayer(playerName).getHoleCards().getCards();

	for (std::size_t i = 0; i < holeCards.size(); i++)
	{
		std::size_t xOffset = static_cast<std::size_t>(i * this->data.window.getSize().x / 12.f);
		this->cardSprites.at(holeCards.at(i).toString()).setPosition(sf::Vector2f(playerLabel->second->GetAbsolutePosition().x + xOffset, playerLabel->second->GetAbsolutePosition().y - (this->data.window.getSize().y / 5.f)));
		this->data.window.draw(this->cardSprites.at(holeCards.at(i).toString()));
	}
}
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
	playerItems(),
	cardSprites(),
	dealer(data.pokerTable),
	pokerAI(dealer),
	mainPlayerName(data.pokerTable.getPlayers().at(data.pokerTable.getSize() > 2 ? 2 : 0).getName()),
	mainPlayerActed(false)
{
	data.textures.loadTexture("PokerTable", "Resources/PokerTable.png");
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
		[this, data] { dealer.makeFold(dealer.getPokerTable().findPlayer(mainPlayerName));
	actionFrame->Show(false);
	mainPlayerActed = true;
	dealer.updateGameStage();
	});
	
	callButton = sfg::Button::Create("    Call    ");
	callButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, data] { dealer.makeCall(dealer.getPokerTable().findPlayer(mainPlayerName)); 
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
	potLabel->SetPosition(sf::Vector2f(data.window.getSize().x / 2.7f, data.window.getSize().y / 1.8f));

	const auto& players = dealer.getPokerTable().getPlayers();

	for (auto& player : players)
	{
		playerItems.push_back(PlayerItem(player.getName(), sfg::Label::Create(player.toString())));
	}

	positionPlayerItems();
	assignCardTextures();
	
	hud.Add(menuButton);
	hud.Add(tableLabel);
	hud.Add(potLabel);
	hud.Add(actionFrame);
	
	for (const auto& playerItem : playerItems)
	{
		hud.Add(playerItem.label);
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

	this->updatePlayerItems();
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
	this->drawHoleCards();
	this->data.sfgui.Display(this->data.window);
}

PlayState::PlayerItem::PlayerItem(const std::string& name, sfg::Label::Ptr label) :
	name(name),
	label(label)
{
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

	for (const auto& playerItem : this->playerItems)
	{
		playerItem.label->Show(false);
	}

	this->states.popState();
}

void PlayState::processBet()
{
	try
	{
		this->dealer.makeBet(this->dealer.getPokerTable().findPlayer(mainPlayerName), static_cast<std::size_t>(std::stoull(betEntry->GetText().toAnsiString())));
	}
	catch (const std::invalid_argument& e)
	{
		this->dealer.makeCall(this->dealer.getPokerTable().findPlayer(mainPlayerName));
	}
}

void PlayState::positionPlayerItems()
{
	std::size_t labelPosition = this->playerItems.size() > 2 ? 2 : 0;

	for (std::size_t i = 0; i < playerItems.size(); i++, labelPosition++)
	{
		if (labelPosition == playerItems.size())
		{
			labelPosition = 0;
		}

		const float PI = static_cast<float>(std::atan(1)) * 4;
		float xRadius = this->data.window.getSize().x / 2.5f;
		float yRadius = this->data.window.getSize().y / 2.9f;
		float theta = (2 * PI * i) / this->playerItems.size() + (PI / 2);
		this->playerItems.at(labelPosition).label->SetPosition(sf::Vector2f(std::round(xRadius * std::cos(theta)) + this->data.window.getSize().x / 2.2f, std::round(yRadius * std::sin(theta)) + this->data.window.getSize().y / 2.f));
	}
}

void PlayState::adjustBetEntry()
{
	this->betEntry->SetText(std::to_string(static_cast<std::size_t>(this->betAdjustment->GetValue() / (this->betAdjustment->GetUpper() - this->betAdjustment->GetPageSize()) * this->dealer.getPokerTable().getPlayer(this->mainPlayerName).getStack())));
}

void PlayState::updatePlayerItems()
{
	const auto& players = this->dealer.getPokerTable().getPlayers();

	for (auto& playerItem : this->playerItems)
	{
		auto player = std::find_if(players.begin(), players.end(),
			[&](const Player& player) { return player.getName() == playerItem.name; });

		if (player != players.end())
		{
			playerItem.label->SetText(player->toString());
		}
		else
		{
			playerItem.label->Show(false);
		}
	}
}

void PlayState::updatePotLabel()
{
	const auto& pokerTable = this->dealer.getPokerTable();
	std::string callAmount = "";

	if (pokerTable.findPlayer(this->mainPlayerName) < pokerTable.getSize())
	{
		callAmount = "Call Amount: " + std::to_string(this->dealer.maxContribution() - this->dealer.getPokerTable().getPlayer(this->mainPlayerName).getPotContribution()) + " |";
	}

	potLabel->SetText("| Pot: " + std::to_string(this->dealer.getPokerTable().getPot()) + " | " + callAmount);
}

void PlayState::assignCardTextures()
{
	const auto& cards = Deck().CardContainer::getCards();

	this->data.textures.loadTexture("CardBack", "Resources/CardBack.png");

	for (std::size_t i = 1; i <= 52; i++)
	{
		const auto& card = cards.at(i - 1).toString();

		if (!this->data.textures.hasTexture(card))
		{
			this->data.textures.loadTexture(card, "Resources/Card" + std::to_string(i) + ".png");
		}

		sf::Sprite cardSprite(this->data.textures.getTexture(card));
		cardSprite.setScale(0.26f, 0.28f);

		this->cardSprites.insert(std::make_pair(card, cardSprite));
	}

	for (std::size_t i = 0; i < this->playerItems.size(); i++)
	{
		for (std::size_t j = 0; j < this->dealer.getPokerTable().getPlayer(this->mainPlayerName).getHoleCards().getSize(); j++)
		{
			sf::Sprite backCardSprite(this->data.textures.getTexture("CardBack"));
			const std::size_t xOffset = static_cast<std::size_t>(j * this->data.window.getSize().x / 18.f);

			backCardSprite.setScale(0.12f, 0.15f);
			backCardSprite.setPosition(sf::Vector2f(this->playerItems.at(i).label->GetAbsolutePosition().x + xOffset, playerItems.at(i).label->GetAbsolutePosition().y - (this->data.window.getSize().y / 8.f)));
			playerItems.at(i).hiddenHoleCards.push_back(backCardSprite);
		}
	}
}

void PlayState::drawCommunityCards()
{
	if (this->dealer.getPokerTable().activePlayerCount() > 1)
	{
		const auto& communityCards = this->dealer.getCommunityCards().getCards();

		for (std::size_t i = 0; i < communityCards.size(); i++)
		{
			const std::size_t xOffset = static_cast<std::size_t>(i * (this->data.window.getSize().x / 10.f));
			this->cardSprites.at(communityCards.at(i).toString()).setPosition(sf::Vector2f((this->data.window.getSize().x / 3.6f) + xOffset, this->data.window.getSize().y / 3.f));
			this->data.window.draw(this->cardSprites.at(communityCards.at(i).toString()));
		}
	}
}

void PlayState::drawHoleCards()
{
	const auto& pokerTable = this->dealer.getPokerTable();
	const auto& players = pokerTable.getPlayers();

	for (std::size_t i = 0; i < this->playerItems.size(); i++)
	{
		const auto& player = std::find_if(players.begin(), players.end(),
			[&](const Player& player) { return player.getName() == this->playerItems.at(i).name; });

		if (player != players.end())
		{
			for (std::size_t j = 0; j < player->getHoleCards().getSize(); j++)
			{
				const std::size_t xOffset = static_cast<std::size_t>(j * this->data.window.getSize().x / 12.f);
				const auto& cardPosition = sf::Vector2f(this->playerItems.at(i).label->GetAbsolutePosition().x + xOffset, playerItems.at(i).label->GetAbsolutePosition().y - (this->data.window.getSize().y / 5.f));

				if (this->playerItems.at(i).name == this->mainPlayerName)
				{
					const auto& mainPlayerHoleCards = pokerTable.getPlayer(this->mainPlayerName).getHoleCards().getCards();
					this->cardSprites.at(mainPlayerHoleCards.at(j).toString()).setPosition(cardPosition);
					this->data.window.draw(this->cardSprites.at(mainPlayerHoleCards.at(j).toString()));
				}
				else 
				{
					this->data.window.draw(this->playerItems.at(i).hiddenHoleCards.at(j));
				}
			}
		}
	}
}
/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Dealer.cpp
InversePalindrome.com
*/


#include <algorithm>
#include "Dealer.hpp"


Dealer::Dealer(const PokerTable& pokerTable) :
	Dealer(pokerTable, Deck())
{
}

Dealer::Dealer(const PokerTable& pokerTable, const Deck& deck) :
	pokerTable(pokerTable),
	deck(deck),
	communityCards(),
	muckedCards()
{
}

PokerTable Dealer::getPokerTable() const
{
	return this->pokerTable;
}

Deck Dealer::getDeck() const
{
	return this->deck;
}

CardContainer Dealer::getCommunityCards() const
{
	return this->communityCards;
}

CardContainer Dealer::getMuckedCards() const
{
	return this->muckedCards;
}

void Dealer::setPokerTable(const PokerTable& pokerTable)
{
	this->pokerTable = pokerTable;
}

void Dealer::setDeck(const Deck& deck)
{
	this->deck = deck;
}

void Dealer::startHand()
{
	for (auto& player : this->pokerTable.players)
	{
		player.setAction(Player::UndefinedAction);
	}

	this->postBlinds();
	this->postAnte();
}

void Dealer::dealPreFlop(std::size_t cardsPerPlayer)
{
	this->deck.shuffle();

	for (auto& player : this->pokerTable.players)
	{
		player.setHoleCards(CardContainer(this->deck.getCards(cardsPerPlayer)));
		this->deck.removeCards(cardsPerPlayer);
	}
}

void Dealer::dealFlop()
{
	this->communityCards.addCards(this->deck.getCards(3));
	this->deck.removeCards(3);
}

void Dealer::dealTurn()
{
	this->communityCards.addCard(this->deck.getCard());
	this->deck.removeCards(1);
}

void Dealer::dealRiver()
{
	this->communityCards.addCard(this->deck.getCard());
	this->deck.removeCards(1);
}

void Dealer::endHand()
{
	this->transferPotToWinner();
	this->transferCardsToDeck();
	this->pokerTable.clearPot();

	this->pokerTable.players.erase(std::remove_if(this->pokerTable.players.begin(), this->pokerTable.players.end(),
			[&](const Player& player) {return player.getStack() < this->pokerTable.getBigBlind(); }), this->pokerTable.players.end());

	this->changePositions();
}

void Dealer::transferChipsFromPlayerToPot(std::size_t playerPosition, std::size_t chips)
{
	if (playerPosition >= 0 && playerPosition < this->pokerTable.getSize())
	{
		this->pokerTable.players.at(playerPosition).removeFromStack(chips);
		this->pokerTable.players.at(playerPosition).addToPotContribution(chips);
		this->pokerTable.addToPot(chips);
	}
}

void Dealer::makeFold(std::size_t playerPosition)
{
	this->muckedCards.addCards(this->pokerTable.players.at(playerPosition).getHoleCards().getCards());

	this->pokerTable.players.at(playerPosition).setAction(Player::Fold);
	this->pokerTable.players.at(playerPosition).clearHoleCards();
}

void Dealer::makeCall(std::size_t playerPosition)
{
	this->pokerTable.players.at(playerPosition).setAction(Player::Call);
	this->transferChipsFromPlayerToPot(playerPosition, this->maxContribution() - this->pokerTable.players.at(playerPosition).getPotContribution());
}

void Dealer::makeBet(std::size_t playerPosition, std::size_t chips)
{
	if (this->pokerTable.players.at(playerPosition).getPotContribution() + chips > this->maxContribution())
	{
		this->pokerTable.players.at(playerPosition).setAction(Player::Bet);
		this->transferChipsFromPlayerToPot(playerPosition, chips);
	}
}

void Dealer::changePositions()
{
	std::rotate(this->pokerTable.players.rbegin(), this->pokerTable.players.rbegin() + 1, this->pokerTable.players.rend());
}

void Dealer::transferPotToWinner()
{
	std::size_t numOfPlayersContributions = std::count_if(this->pokerTable.players.begin(), this->pokerTable.players.end(),
		[&](const Player& player) { return player.getPotContribution() > 0;  });

	while (numOfPlayersContributions > 0)
	{
		for (auto& player : this->pokerTable.players)
		{
			if (player.getPotContribution() == 0)
			{
				this->makeFold(this->pokerTable.findPlayer(player));
			}
		}

		std::size_t minContribution = this->minContribution();

		for (auto& player : this->pokerTable.players)
		{
			player.removeFromPotContribution(minContribution);
		}

		std::size_t sidePot = minContribution * numOfPlayersContributions;

		const auto& showdownHands = this->rankedPokerHands();
		std::size_t numOfWinners = std::count_if(this->pokerTable.players.begin(), this->pokerTable.players.end(),
			[&](const Player& player) { return PokerHand(player.getHoleCards().getCards(), this->communityCards.getCards()) == showdownHands.back(); });

		for (auto& player : this->pokerTable.players)
		{
			if (player.isActive() && PokerHand(player.getHoleCards().getCards(), this->communityCards.getCards()) == showdownHands.back())
			{
				player.addToStack(sidePot / numOfWinners);
			}
		}

		numOfPlayersContributions = std::count_if(this->pokerTable.players.begin(), this->pokerTable.players.end(),
			[&](const Player& player) { return player.getPotContribution() > 0; });
	}

	for (auto& player : this->pokerTable.players)
	{
		if (player.isActive())
		{
			this->makeFold(this->pokerTable.findPlayer(player));
		}
	}
}

void Dealer::transferCardsToDeck()
{
	this->deck.addCards(this->communityCards.getCards());
	this->deck.addCards(this->muckedCards.getCards());
	this->communityCards.clearCards();
	this->muckedCards.clearCards();
}

void Dealer::postBlinds()
{
	this->transferChipsFromPlayerToPot(0, this->pokerTable.bigBlind / 2);
	this->transferChipsFromPlayerToPot(1, this->pokerTable.bigBlind);
}

void Dealer::postAnte()
{
	for (std::size_t i = 0; i < this->pokerTable.players.size(); i++)
	{
		this->transferChipsFromPlayerToPot(i, this->pokerTable.ante);
	}
}

std::size_t Dealer::minContribution() const
{
	std::size_t minContribution = this->pokerTable.pot;

	for (const auto& player : this->pokerTable.players)
	{
		if (player.getPotContribution() > 0 && player.getPotContribution() < minContribution)
		{
			minContribution = player.getPotContribution();
		}
	}

	return minContribution;
}

std::size_t Dealer::maxContribution() const
{
	std::size_t maxContribution = 0;

	for (const auto& player : this->pokerTable.players)
	{
		if (maxContribution < player.getPotContribution())
		{
			maxContribution = player.getPotContribution();
		}
	}

	return maxContribution;
}

std::vector<PokerHand> Dealer::rankedPokerHands() const
{
	std::vector<PokerHand> showdownHands;

	for (const auto& player : this->pokerTable.players)
	{
		if (player.isActive())
		{
			showdownHands.push_back(PokerHand(player.getHoleCards().getCards(), this->communityCards.getCards()));
		}
	}

	std::sort(showdownHands.begin(), showdownHands.end());

	return showdownHands;
}
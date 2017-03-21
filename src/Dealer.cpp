/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Dealer.cpp
InversePalindrome.com
*/


#include "Dealer.hpp"


Dealer::Dealer(const PokerTable& pokerTable) :
	pokerTable(pokerTable),
	deck(),
	communityCards(),
	muckedCards()
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

void Dealer::postBlinds()
{
	this->transferChipsFromPlayerToPot(0, this->pokerTable.bigBlind / 2);
	this->transferChipsFromPlayerToPot(1, this->pokerTable.bigBlind);
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

	this->pokerTable.players.erase(std::remove_if(this->pokerTable.players.begin(), this->pokerTable.players.end(),
			[&](const Player& player) {return player.getStack() < this->pokerTable.getBigBlind(); }), this->pokerTable.players.end());
}

void Dealer::transferChipsFromPlayerToPot(std::size_t playerPosition, std::size_t chips)
{
	if (playerPosition >= 0 && playerPosition < this->pokerTable.getSize())
	{
		if (this->pokerTable.players.at(playerPosition).getStack() > chips)
		{
			this->pokerTable.players.at(playerPosition).removeFromStack(chips);
			this->pokerTable.addToPot(chips);
		}
		else
		{
			this->pokerTable.addToPot(this->pokerTable.players.at(playerPosition).getStack());
			this->pokerTable.players.at(playerPosition).removeAllStack();
		}
	}
}

void Dealer::transferPotToWinner()
{
	std::vector<PokerHand> showdownHands;

	for (auto& player : this->pokerTable.players)
	{
		if (!player.hasFolded())
		{
			showdownHands.push_back(PokerHand(player.getHoleCards().getCards(), this->communityCards.getCards()));
		}
	}

	std::sort(showdownHands.rbegin(), showdownHands.rend());
	std::size_t numOfWinners = std::count(showdownHands.begin(), showdownHands.end(), showdownHands.front());

	for (auto& player : this->pokerTable.players)
	{
		if (!player.hasFolded() &&
			PokerHand(player.getHoleCards().getCards(), this->communityCards.getCards()) == showdownHands.front())
		{
			player.addToStack(this->pokerTable.pot / numOfWinners);
		}
	}

	this->pokerTable.clearPot();
}

void Dealer::transferCardsToDeck()
{
	for (auto& player : this->pokerTable.players)
	{
		if (!player.hasFolded())
		{
			this->deck.addCards(player.getHoleCards().getCards());
			player.removeHoleCards();
		}
	}

	this->deck.addCards(this->communityCards.getCards());
	this->deck.addCards(this->muckedCards.getCards());
	this->communityCards.clearCards();
	this->muckedCards.clearCards();
}
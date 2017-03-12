/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Dealer.cpp
InversePalindrome.com
*/


#include "Dealer.hpp"


Dealer::Dealer(const PokerTable& pokerTable) :
	pokerTable(pokerTable),
	deck(),
	board(),
	muckedCards()
{
}

Dealer::Dealer(const PokerTable& pokerTable, const Deck& deck) :
	pokerTable(pokerTable),
	deck(deck),
	board(),
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

std::vector<Card> Dealer::getBoard() const
{
	return this->board;
}

std::vector<Card> Dealer::getMuckedCards() const
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


void Dealer::dealPreFlop(std::size_t cardsPerPlayer) 
{
	this->deck.shuffle();

	for (Player& player : this->pokerTable.players)
	{
		player.setHoleCards(HoleCards(this->deck.getCards(cardsPerPlayer)));
		this->deck.removeCards(cardsPerPlayer);
	}

	this->transferChipsFromPlayerToPot(0, this->pokerTable.bigBlind / 2);
	this->transferChipsFromPlayerToPot(1, this->pokerTable.bigBlind);
}

void Dealer::dealFlop()
{
	const auto& flop = this->deck.getCards(3);
	this->board.insert(this->board.begin(), flop.begin(), flop.end());
	this->deck.removeCards(3);
}

void Dealer::dealTurn()
{
	const auto& turn = this->deck.getCard();
	this->board.push_back(turn);
	this->deck.removeCards(1);
}

void Dealer::dealRiver()
{
	const auto& river = this->deck.getCard();
	this->board.push_back(river);
	this->deck.removeCards(1);
}

void Dealer::endHand()
{
	this->deck.addCards(this->board);
	this->deck.addCards(this->muckedCards);
	this->board.clear();
    this->muckedCards.clear();

	for (const Player& player : this->pokerTable.players)
	{
		if (player.getStack() < pokerTable.getBigBlind())
		{
			this->pokerTable.removePlayer(player);
		}
	}
}

void Dealer::transferChipsFromPlayerToPot(std::size_t playerPosition, std::size_t chips)
{
	if (playerPosition >= 0 && playerPosition < this->pokerTable.getSize())
	{
		if (!this->pokerTable.players.at(playerPosition).getStack() < chips)
		{
			this->pokerTable.players.at(playerPosition).removeFromStack(chips);
			this->pokerTable.addToPot(chips);
		}
		else
		{
			this->pokerTable.players.at(playerPosition).setPlayingStatus(false);
		}
	}
}

std::string Dealer::boardToString() const
{
	std::string boardRepresentation = "| ";

	for (const auto& card : this->board)
	{
		boardRepresentation += card.toString() + " | ";
	}

	return boardRepresentation;
}


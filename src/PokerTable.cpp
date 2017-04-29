/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PokerTable.cpp
InversePalindrome.com
*/


#include <algorithm>
#include "PokerTable.hpp"


PokerTable::PokerTable(const std::vector<Player>& players) :
	PokerTable(players, 10, 0)
{
}

PokerTable::PokerTable(const std::vector<Player>& players, std::size_t bigBlind) :
	PokerTable(players, bigBlind, 0)
{
}

PokerTable::PokerTable(const std::vector<Player>& players, std::size_t bigBlind, std::size_t ante) :
	players(players),
	bigBlind(bigBlind),
	ante(ante),
	pot(0)
{
}

Player PokerTable::getPlayer(const std::string& playerName) const
{
	return this->players.at(std::distance(this->players.begin(), std::find_if(this->players.begin(), this->players.end(), 
		[&](const Player& player) { return player.getName() == playerName; })));
}

std::vector<Player> PokerTable::getPlayers() const
{
	return this->players;
}

std::size_t PokerTable::getBigBlind() const
{
	return this->bigBlind;
}

std::size_t PokerTable::getAnte() const
{
	return this->ante;
}

std::size_t PokerTable::getPot() const
{
	return this->pot;
}

std::size_t PokerTable::getSize() const
{
	return this->players.size();
}

void PokerTable::setPlayers(const std::vector<Player>& players)
{
	this->players = players;
}

void PokerTable::setBigBlind(std::size_t chips)
{
	this->bigBlind = chips;
}

void PokerTable::setAnte(std::size_t chips)
{
	this->ante = chips;
}
void PokerTable::setPot(std::size_t chips)
{
	this->pot = chips;
}

void PokerTable::addPlayer(const Player& player)
{
	this->players.push_back(player);
}

void PokerTable::addToPot(std::size_t chips)
{
	this->pot += chips;
}

void PokerTable::addChipsToPlayer(const std::string& playerName, std::size_t chips)
{
	this->players.at(this->findPlayer(playerName)).addToStack(chips);
}

void PokerTable::removePlayer(const std::string& playerName)
{
	this->players.erase(this->players.begin() + this->findPlayer(playerName));
}

void PokerTable::removeChipsFromPlayer(const std::string& playerName, std::size_t chips)
{
	this->players.at(this->findPlayer(playerName)).removeFromStack(chips);
}

void PokerTable::insertPlayer(const Player& player, std::size_t playerPosition)
{
	this->players.insert(this->players.begin() + playerPosition, player);
}

void PokerTable::clearPlayers()
{
	this->players.clear();
}

void PokerTable::clearPot()
{
	this->pot = 0;
}

std::size_t PokerTable::findPlayer(const std::string& playerName) const
{
	return std::distance(this->players.begin(), std::find_if(this->players.begin(), this->players.end(), 
		[&](const Player& player2) { return playerName == player2.getName(); }));
}

std::size_t PokerTable::activePlayerCount() const
{
	return std::count_if(this->players.begin(), this->players.end(),
		[&](const Player& player) { return player.isActive() && player.getStack() > 0; });
}

std::size_t PokerTable::allInCount() const
{
	return std::count_if(this->players.begin(), this->players.end(),
		[&](const Player& player) { return player.isActive() && player.getStack() == 0; });
}

bool PokerTable::hasPlayers() const
{
	return !this->players.empty();
}

std::string PokerTable::toString() const
{
	return "| " + std::to_string(this->bigBlind / 2) + " / " + std::to_string(this->bigBlind) + " blinds | " + std::to_string(this->ante) + " ante |";
}
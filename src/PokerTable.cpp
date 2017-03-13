/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PokerTable.cpp
InversePalindrome.com
*/


#include "PokerTable.hpp"


PokerTable::PokerTable(const std::vector<Player>& players, std::size_t bigBlind) :
	players(players),
	bigBlind(bigBlind),
	pot(0)
{
}

std::vector<Player> PokerTable::getPlayers() const
{
	return this->players;
}

std::size_t PokerTable::getBigBlind() const
{
	return this->bigBlind;
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

void PokerTable::setPot(std::size_t chips)
{
	this->pot = chips;
}

void PokerTable::addPlayer(const Player& player)
{
	this->players.push_back(player);
}

void PokerTable::removePlayer(const Player& player)
{
	this->players.erase(std::remove(this->players.begin(), this->players.end(), player), this->players.end());
}

void PokerTable::removePlayers()
{
	this->players.clear();
}

void PokerTable::addToPot(std::size_t chips)
{
	this->pot += chips;
}

void PokerTable::clearPot()
{
	this->pot = 0;
}

std::size_t PokerTable::findPlayer(const Player& player) const
{
	return std::distance(this->players.begin(), std::find(this->players.begin(), this->players.end(), player));
}

void PokerTable::addChipsFromPlayer(const Player& player, std::size_t chips)
{
	this->players.at(findPlayer(player)).addToStack(chips);
}

void PokerTable::removeChipsFromPlayer(const Player& player, std::size_t chips)
{
	this->players.at(findPlayer(player)).removeFromStack(chips);
}

bool PokerTable::hasPlayers() const
{
	return !this->players.empty();
}

std::string PokerTable::toString() const
{
	return "| Table | " + std::to_string(this->players.size()) + " players | " + std::to_string(this->bigBlind / 2) + " / " + std::to_string(this->bigBlind) + " | ";
}
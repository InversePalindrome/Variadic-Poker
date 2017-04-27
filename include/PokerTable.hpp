/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PokerTable.hpp
InversePalindrome.com
*/


#pragma once
#include <vector>
#include <string>
#include "Player.hpp"


class PokerTable
{
	friend class Dealer;

public:
	PokerTable(const std::vector<Player>& players);
	PokerTable(const std::vector<Player>& players, std::size_t bigBlind);
	PokerTable(const std::vector<Player>& players, std::size_t bigBlind, std::size_t ante);

	Player getPlayer(const std::string& playerName) const;
	std::vector<Player> getPlayers() const;
	std::size_t getBigBlind() const;
	std::size_t getAnte() const;
	std::size_t getPot() const;
	std::size_t getSize() const;

	void setPlayers(const std::vector<Player>& players);
	void setBigBlind(std::size_t chips);
	void setAnte(std::size_t chips);
	void setPot(std::size_t chips);

	void addPlayer(const Player& player);
	void addToPot(std::size_t chips);
	void addChipsToPlayer(const Player& player, std::size_t chips);

	void removePlayer(const Player& player);
	void removeChipsFromPlayer(const Player& player, std::size_t chips);

	void insertPlayer(const Player& player, std::size_t playerPosition);

	void clearPlayers();
	void clearPot();

	std::size_t findPlayer(const Player& player) const;
	
	std::size_t activePlayerCount() const;
	std::size_t allInCount() const;

	bool hasPlayers() const;

	std::string toString() const;

private:
	std::vector<Player> players;
	std::size_t bigBlind;
	std::size_t ante;
	std::size_t pot;
};
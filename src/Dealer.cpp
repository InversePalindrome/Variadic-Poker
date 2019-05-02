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
    muckedCards(),
    street(Street::UndefinedStreet),
    currentPosition(0),
    actionCount(0)
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

Dealer::Street Dealer::getStreet() const
{
    return this->street;
}

std::size_t Dealer::getCurrentPosition() const
{
    return this->currentPosition;
}

void Dealer::setPokerTable(const PokerTable& pokerTable)
{
    this->pokerTable = pokerTable;
}

void Dealer::setDeck(const Deck& deck)
{
    this->deck = deck;
}

void Dealer::setStreet(Street street)
{
    this->street = street;
}

void Dealer::setCurrentPosition(std::size_t currentPosition)
{
    this->currentPosition = currentPosition;
}

void Dealer::startHand()
{
    for (auto& player : this->pokerTable.players)
    {
        player.setAction(Player::UndefinedAction);
    }

    this->postBlinds();
    this->postAnte();
    this->deck.shuffle();
}

void Dealer::dealPreFlop(std::size_t cardsPerPlayer)
{
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
        [&](const Player & player) { return player.getStack() == 0; }), this->pokerTable.players.end());

    this->changePositions();
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
    this->actionCount++;
}

void Dealer::makeBet(std::size_t playerPosition, std::size_t chips)
{
    if (this->pokerTable.players.at(playerPosition).getPotContribution() + chips > this->maxContribution())
    {
        this->pokerTable.players.at(playerPosition).setAction(Player::Bet);
        this->transferChipsFromPlayerToPot(playerPosition, chips);
        this->actionCount++;
    }
    else
    {
        this->makeCall(playerPosition);
    }
}

void Dealer::updateGameStage()
{
    bool hasMovedPosition = false;

    std::size_t activePlayerCount = this->pokerTable.activePlayerCount();
    std::size_t allInCount = this->pokerTable.allInCount();

    switch (this->street)
    {
    case UndefinedStreet:
        this->currentPosition = this->pokerTable.getSize() > 2 ? 2 : 0;
        hasMovedPosition = true;
        this->startHand();
        this->dealPreFlop(2);
        this->street = Preflop;
        break;
    case Preflop:
        if (this->roundEnded())
        {
            this->actionCount = 0;
            this->moveCurrentPosition();
            hasMovedPosition = true;
            this->dealFlop();
            this->street = Flop;
            if (activePlayerCount > 1)
            {
                break;
            }
            else
            {
                this->actionCount = activePlayerCount + allInCount;
            }
        }
    case Flop:
        if (this->roundEnded())
        {
            this->actionCount = 0;
            this->moveCurrentPosition();
            hasMovedPosition = true;
            this->dealTurn();
            this->street = Turn;
            if (activePlayerCount > 1)
            {
                break;
            }
            else
            {
                this->actionCount = activePlayerCount + allInCount;
            }
        }
    case Turn:
        if (this->roundEnded())
        {
            this->actionCount = 0;
            this->moveCurrentPosition();
            hasMovedPosition = true;
            this->dealRiver();
            this->street = River;
            if (activePlayerCount > 1)
            {
                break;
            }
            else
            {
                this->actionCount = activePlayerCount + allInCount;
            }
        }
    case River:
        if (this->roundEnded())
        {
            this->endHand();
            this->actionCount = 0;
            this->currentPosition = this->pokerTable.getSize() > 2 ? 2 : 0;
            hasMovedPosition = true;
            this->startHand();
            this->dealPreFlop(2);
            this->street = Preflop;
        }
        break;
    }

    if (!hasMovedPosition)
    {
        this->moveCurrentPosition();
    }
}

std::size_t Dealer::minContribution() const
{
    std::size_t minContribution = this->pokerTable.pot;

    for (const auto& player : this->pokerTable.players)
    {
        if (minContribution > player.getPotContribution() && player.getPotContribution() > 0)
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

void Dealer::changePositions()
{
    std::rotate(this->pokerTable.players.rbegin(), this->pokerTable.players.rbegin() + 1, this->pokerTable.players.rend());
}

void Dealer::moveCurrentPosition()
{
    for (std::size_t i = this->currentPosition + 1; i != this->currentPosition; i++)
    {
        if (i == this->pokerTable.players.size())
        {
            i = 0;
        }

        if (this->pokerTable.players.at(i).isActive() && this->pokerTable.players.at(i).getStack() > 0)
        {
            this->currentPosition = i;
            break;
        }
    }
}

void Dealer::transferChipsFromPlayerToPot(std::size_t playerPosition, std::size_t chips)
{
    if (playerPosition >= 0 && playerPosition < this->pokerTable.getSize())
    {
        std::size_t transferAmount = pokerTable.players.at(playerPosition).getStack() > chips ? chips : pokerTable.players.at(playerPosition).getStack();

        this->pokerTable.players.at(playerPosition).addToPotContribution(transferAmount);
        this->pokerTable.addToPot(transferAmount);
        this->pokerTable.players.at(playerPosition).removeFromStack(transferAmount);
    }
}

void Dealer::transferPotToWinner()
{
    std::size_t numOfPlayersContributions = std::count_if(this->pokerTable.players.begin(), this->pokerTable.players.end(),
        [&](const Player & player) { return player.getPotContribution() > 0;  });

    while (numOfPlayersContributions > 1)
    {
        std::size_t minContribution = this->minContribution();

        for (auto& player : this->pokerTable.players)
        {
            player.removeFromPotContribution(minContribution);
        }

        std::size_t sidePot = minContribution * numOfPlayersContributions;

        auto& showdownHands = this->rankedPokerHands();

        if (!showdownHands.empty())
        {
            std::size_t numOfWinners = std::count_if(this->pokerTable.players.begin(), this->pokerTable.players.end(),
                [&](const Player & player) { return PokerHand(player.getHoleCards().getCards(), this->communityCards.getCards()) == showdownHands.back(); });

            for (auto& player : this->pokerTable.players)
            {
                if (player.isActive() && PokerHand(player.getHoleCards().getCards(), this->communityCards.getCards()) == showdownHands.back())
                {
                    player.addToStack(sidePot / numOfWinners);
                }
            }
        }

        for (const auto& player : this->pokerTable.players)
        {
            if (player.isActive() && player.getPotContribution() == 0)
            {
                this->makeFold(this->pokerTable.findPlayer(player.getName()));
            }
        }

        numOfPlayersContributions = std::count_if(this->pokerTable.players.begin(), this->pokerTable.players.end(),
            [&](const Player & player) { return player.getPotContribution() > 0; });
    }

    if (numOfPlayersContributions == 1)
    {
        auto lastPlayer = std::find_if(this->pokerTable.players.begin(), this->pokerTable.players.end(),
            [&](const Player & player) { return player.getPotContribution() > 0; });

        this->makeFold(this->pokerTable.findPlayer(lastPlayer->getName()));
        lastPlayer->addToStack(lastPlayer->getPotContribution());
        lastPlayer->clearPotContribution();
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

bool Dealer::roundEnded() const
{
    std::size_t activePlayerCount = this->pokerTable.activePlayerCount();
    std::size_t allInCount = this->pokerTable.allInCount();

    std::size_t calledMaxBetCount = 0;

    for (const auto& player : this->pokerTable.players)
    {
        if (player.isActive() && (player.getStack() == 0 || this->maxContribution() == player.getPotContribution()))
        {
            calledMaxBetCount++;
        }
    }

    return this->actionCount >= activePlayerCount + allInCount && (activePlayerCount <= 1 || activePlayerCount + allInCount == calledMaxBetCount);
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
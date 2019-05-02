/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PokerAI.cpp
InversePalindrome.com
*/


#include <random>
#include "PokerAI.hpp"
#include "Deck.hpp"


PokerAI::PokerAI(Dealer& dealer) :
    dealer(dealer)
{
}

void PokerAI::executeAction(std::size_t playerPosition)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double choiceProbability = dist(mt);
    double rateOfReturn = this->calculateRateOfReturn(playerPosition);

    auto playerAction = Player::UndefinedAction;

    if (rateOfReturn < 0.8)
    {
        if (choiceProbability < 0.95)
        {
            playerAction = Player::Fold;
        }
        else
        {
            playerAction = Player::Bet;
        }
    }
    else if (rateOfReturn < 1.0)
    {
        if (choiceProbability < 0.8)
        {
            playerAction = Player::Fold;
        }
        else if (choiceProbability < 0.85)
        {
            playerAction = Player::Call;
        }
        else
        {
            playerAction = Player::Bet;
        }
    }
    else if (rateOfReturn < 1.3)
    {
        if (choiceProbability < 0.6)
        {
            playerAction = Player::Call;
        }
        else
        {
            playerAction = Player::Bet;
        }
    }
    else
    {
        if (choiceProbability < 0.3)
        {
            playerAction = Player::Call;
        }
        else
        {
            playerAction = Player::Bet;
        }
    }

    this->selectAction(playerPosition, playerAction);
}

double PokerAI::calculateRateOfReturn(std::size_t playerPosition) const
{
    return static_cast<double>(this->calculateHandStrength(playerPosition)) / this->calculatePotOdds(playerPosition);
}

double PokerAI::calculatePotOdds(std::size_t playerPosition) const
{
    return static_cast<double>(this->dealer.maxContribution() - this->dealer.getPokerTable().getPlayers().at(playerPosition).getPotContribution()) / this->dealer.getPokerTable().getPot();
}

double PokerAI::calculateHandStrength(std::size_t playerPosition) const
{
    const std::size_t simulationCount = 1000;
    std::size_t ahead = 0, tied = 0, behind = 0;

    auto communityCards = this->dealer.getCommunityCards();
    auto playerHoleCards = this->dealer.getPokerTable().getPlayers().at(playerPosition).getHoleCards().getCards();

    Deck deck;
    deck.CardContainer::removeCards(playerHoleCards);
    deck.CardContainer::removeCards(communityCards.getCards());

    const auto& players = this->dealer.getPokerTable().getPlayers();
    std::size_t activePlayerCount = std::count_if(players.begin(), players.end(), [](const Player & player) { return player.isActive(); });
    std::size_t currentCommunityCardsSize = communityCards.getSize();

    for (std::size_t i = 0; i < simulationCount; i++)
    {
        deck.shuffle();

        auto cards = deck.CardContainer::getCards();
        communityCards.addCards(std::vector<Card>(cards.begin(), cards.begin() + 5 - currentCommunityCardsSize));

        for (std::size_t j = 0; j < activePlayerCount - 1; j++)
        {
            auto opponentHand = PokerHand(std::vector<Card>(cards.begin() + (2 * j) + 5 - currentCommunityCardsSize, cards.begin() + (2 * j) + 7 - currentCommunityCardsSize), communityCards.getCards());
            auto playerHand = PokerHand(playerHoleCards, communityCards.getCards());

            if (playerHand > opponentHand)
            {
                ahead++;
            }
            else if (playerHand == opponentHand)
            {
                tied++;
            }
            else
            {
                behind++;
            }
        }

        communityCards.removeCards(5 - currentCommunityCardsSize);
    }

    return static_cast<double>(ahead + (static_cast<double>(tied) / 2)) / (ahead + tied + behind);
}

void PokerAI::selectAction(std::size_t playerPosition, Player::Action playerAction)
{
    bool canCheck = (this->dealer.maxContribution() - this->dealer.getPokerTable().getPlayers().at(playerPosition).getPotContribution()) == 0;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.25, 0.76);

    double potPercentage = dist(mt);

    switch (playerAction)
    {
    case Player::Fold:
        if (canCheck)
        {
            this->dealer.makeCall(playerPosition);
        }
        else
        {
            this->dealer.makeFold(playerPosition);
        }
        break;
    case Player::Call:
        this->dealer.makeCall(playerPosition);
        break;
    case Player::Bet:
        this->dealer.makeBet(playerPosition, static_cast<std::size_t>(this->dealer.getPokerTable().getPot() * potPercentage));
        break;
    }
}
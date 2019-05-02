/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Deck.cpp
InversePalindrome.com
*/


#include <algorithm>
#include <random>
#include "Deck.hpp"


Deck::Deck() :
    CardContainer()
{
    for (int suit = Card::Spade; suit <= Card::Heart; suit++)
    {
        for (int rank = Card::Two; rank <= Card::Ace; rank++)
        {
            addCard(Card(static_cast<Card::Suit>(suit), static_cast<Card::Rank>(rank)));
        }
    }
}

Deck::Deck(const std::vector<Card>& deck) :
    CardContainer(deck)
{
}

std::vector<Card> Deck::getCards(std::size_t numOfCards) const
{
    if (numOfCards > 0 && numOfCards <= this->getSize())
    {
        return std::vector<Card>(this->cards.end() - numOfCards, this->cards.end());
    }

    return{};
}

Card Deck::getCard() const
{
    return this->cards.back();
}

void Deck::shuffle()
{
    std::random_device rd;
    std::mt19937 mt(rd());

    std::shuffle(this->cards.begin(), this->cards.end(), mt);
}
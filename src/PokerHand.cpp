/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PokerHand.cpp
InversePalindrome.com
*/


#include <algorithm>
#include <numeric>
#include "PokerHand.hpp"


PokerHand::PokerHand(const std::vector<Card>& handCards) :
	PokerHand(handCards, {})
{
}

PokerHand::PokerHand(const std::vector<Card>& communityCards, const std::vector<Card>& holeCards) :
	CardContainer(communityCards),
	handStrength(UndefinedHand),
	kicker(0),
	rankTotal{}
{
	addCards(holeCards);
	sort();
	cards.erase(std::unique(cards.begin(), cards.end()), cards.end());
	initializeRankTotal();
	classifyHand();
}

PokerHand::HandStrength PokerHand::getHandStrength() const
{
	return this->handStrength;
}

void PokerHand::classifyHand()
{
	if (findRoyalFlush())
	{
		this->handStrength = HandStrength::RoyalFlush;
		this->kicker = this->findRoyalFlush();
	}
	else if (findStraightFlush())
	{
		this->handStrength = HandStrength::StraightFlush;
		this->kicker = this->findStraightFlush();
	}
	else if (findFourOfAKind())
	{
		this->handStrength = HandStrength::FourOfAKind;
		this->kicker = (this->findFourOfAKind() * 100) + findHighCard(1);
	}
	else if (findFullHouse())
	{
		this->handStrength = HandStrength::FullHouse;
		this->kicker = (this->findFullHouse() * 100) + this->findPair();
	}
	else if (findFlush())
	{
		this->handStrength = HandStrength::Flush;
		this->kicker = this->findFlush();
	}
	else if (findStraight())
	{
		this->handStrength = HandStrength::Straight;
		this->kicker = this->findStraight();
	}
	else if (findThreeOfAKind())
	{
		this->handStrength = HandStrength::ThreeOfAKind;
		this->kicker = (this->findThreeOfAKind() * 100) + (findHighCard(1) * 10) + findHighCard(2);
	}
	else if (findTwoPair())
	{
		this->handStrength = HandStrength::TwoPair;
		this->kicker = (findPair() * 1000) + (findTwoPair() * 100) + findHighCard(1);
	}
	else if (findPair())
	{
		this->handStrength = HandStrength::Pair;
		this->kicker = (findPair() * 1000) + (findHighCard(1) * 100) + (findHighCard(2) * 10) + findHighCard(3);
	}
	else if (findHighCard(5))
	{
		this->handStrength = HandStrength::HighCard;
		this->kicker = (findHighCard(1) * 10000) + (findHighCard(2) * 1000) + (findHighCard(3) * 100) + (findHighCard(2) * 10) + findHighCard(1);
	}
	else
	{
		this->handStrength = HandStrength::UndefinedHand;
	}
}

std::size_t PokerHand::findHighCard(std::size_t nthHighCard) const
{
	auto it = this->rankTotal.begin();

	for (std::size_t i = 0; i < nthHighCard; i++)
	{
		it = std::find(it, this->rankTotal.end(), 1);

		if (it == this->rankTotal.end())
		{
			break;
		}
		else if (i == nthHighCard - 1)
		{
			return Card::RANK_LENGTH - std::distance(this->rankTotal.begin(), it);
		}

		it++;
	}
		
	return 0;
}

std::size_t PokerHand::findPair() const
{
	return Card::RANK_LENGTH - std::distance(this->rankTotal.begin(), std::find(this->rankTotal.begin(), this->rankTotal.end(), 2));
}

std::size_t PokerHand::findTwoPair() const
{
	if (std::count(this->rankTotal.begin(), this->rankTotal.end(), 2) > 1)
	{
		return Card::RANK_LENGTH - std::distance(this->rankTotal.begin(), std::find(std::next(this->rankTotal.begin(), Card::RANK_LENGTH - findPair() + 1), this->rankTotal.end(), 2));
	}

	return 0;
}

std::size_t PokerHand::findThreeOfAKind() const
{
	return Card::RANK_LENGTH - std::distance(this->rankTotal.begin(), std::find(this->rankTotal.begin(), this->rankTotal.end(), 3));
}

std::size_t PokerHand::findStraight() const
{
	std::size_t consecutiveCount = 0;
	
	//checking for regular straight
	for (std::size_t i = 0; i < rankTotal.size(); i++)
	{
		rankTotal[Card::RANK_LENGTH - 1 - i] > 0 ? consecutiveCount++ : consecutiveCount = 0;

		if (consecutiveCount >= 5)
		{
			return i + 1;
		}
	}

	consecutiveCount = 0;

	//checking for wheel based straight
	if (rankTotal[0] > 0)
	{
		for (std::size_t i = Card::RANK_LENGTH - 1; i > Card::RANK_LENGTH - 5; i--)
		{
			rankTotal[i] > 0 ? consecutiveCount++ : consecutiveCount = 0;

			if (consecutiveCount == 4)
			{
				return 4;
			}
		}
	}

	return 0;
}

std::size_t PokerHand::findFlush() const
{
	for (std::size_t i = 0; i < Card::SUIT_LENGTH; i++)
	{
		if (std::count_if(this->cards.begin(), this->cards.end(), [&](const Card& card) { return card.getSuit() == static_cast<Card::Suit>(i + 1); }) >= 5)
		{
			return static_cast<std::size_t>(this->cards.at(std::distance(this->cards.begin(), std::find_if(this->cards.begin(), this->cards.end(),
				[&](const Card& card) { return card.getSuit() == static_cast<Card::Suit>(i + 1); }))).getRank());
		}  
	}

	return 0;
}
	
std::size_t PokerHand::findFullHouse() const
{
	if (this->findPair() && this->findThreeOfAKind())
	{
		return this->findThreeOfAKind();
	}

	return 0;
}

std::size_t PokerHand::findFourOfAKind() const
{
	return Card::RANK_LENGTH - std::distance(this->rankTotal.begin(), std::find(this->rankTotal.begin(), this->rankTotal.end(), 4));
}

std::size_t PokerHand::findStraightFlush() const
{
	if (findStraight() && findFlush())
	{
		std::vector<Card> flushCards = this->cards;

		for (std::size_t i = 0; i < Card::SUIT_LENGTH; i++)
		{
			if (std::count_if(flushCards.begin(), flushCards.end(), [&](const Card& card) { return card.getSuit() == static_cast<Card::Suit>(i + 1); }) >= 5)
			{
				flushCards.erase(std::remove_if(flushCards.begin(), flushCards.end(), [&](const Card& card) { return card.getSuit() != static_cast<Card::Suit>(i + 1); }), flushCards.end());
				break;
			}
		}

		std::size_t consecutiveCount = 0;

		//checking for regular straight flush
		for (std::size_t i = 0; i < flushCards.size() - 1; i++)
		{
			static_cast<std::size_t>(flushCards.at(i).getRank()) == static_cast<std::size_t>(flushCards.at(i + 1).getRank()) + 1 ?
				consecutiveCount++ : consecutiveCount = 0;

			if (consecutiveCount == 4)
			{
				return static_cast<std::size_t>(flushCards.at(i + 1 - consecutiveCount).getRank());
			}
		}

		consecutiveCount = 0;

		if (flushCards.at(0).getRank() == Card::Ace)
		{
			//checking for wheel straight flush
			for (std::size_t i = flushCards.size() - 1; i > flushCards.size() - 4; i--)
			{
				static_cast<std::size_t>(flushCards.at(i).getRank()) + 1 == static_cast<std::size_t>(flushCards.at(i - 1).getRank()) ?
					consecutiveCount++ : consecutiveCount == 0;

				if (consecutiveCount == 3)
				{
					return 4;
				}
			}
		}
	}

	return 0;
}

std::size_t PokerHand::findRoyalFlush() const
{
	if (this->findStraightFlush() == Card::RANK_LENGTH)
	{
		return Card::RANK_LENGTH;
	}

	return 0;
}

std::string PokerHand::toString() const
{
	std::string handRepresentation = "| ";

	switch (this->handStrength)
	{
	case HighCard:
		handRepresentation += this->toString(static_cast<Card::Rank>(this->findHighCard(1))) + " High ";
		break;
	case Pair:
		handRepresentation += "Pair of " + this->toString(static_cast<Card::Rank>(this->findPair())) + "s ";
		break;
	case TwoPair:
		handRepresentation += "Two Pair of " + this->toString(static_cast<Card::Rank>(this->findPair())) + "s and " + 
			this->toString(this->findTwoPair()) + "s ";
		break;
	case ThreeOfAKind:
		handRepresentation += "Three of a Kind " + this->toString(static_cast<Card::Rank>(this->findThreeOfAKind())) + "s ";
		break;
	case Straight:
		handRepresentation += this->toString(static_cast<Card::Rank>(this->kicker)) + " High Straight ";
		break;
	case Flush:
		handRepresentation += this->toString(static_cast<Card::Rank>(this->kicker)) + " High Flush ";
		break;
	case FullHouse:
		handRepresentation += this->toString(static_cast<Card::Rank>(this->findFullHouse())) + "s Full of " + 
			this->toString(static_cast<Card::Rank>(this->findPair())) + "s ";
		break;
	case FourOfAKind:
		handRepresentation += "Four of a Kind " + this->toString(static_cast<Card::Rank>(this->kicker)) + "s ";
		break;
	case StraightFlush:
		handRepresentation += this->toString(static_cast<Card::Rank>(this->kicker)) + " High Straight Flush ";
		break;
	case RoyalFlush:
		handRepresentation += "Royal Flush ";
		break;
	default:
		handRepresentation += "Undefined Hand ";
	}

	return handRepresentation + "|";
}

std::string PokerHand::toString(std::size_t rank) const
{
	switch (rank)
	{
	case 1:
		return "Deuce";
	case 2:
		return "Trey";
	case 3:
		return "Four";
	case 4:
		return "Five";
	case 5:
		return "Six";
	case 6:
		return "Seven";
	case 7:
		return "Eight";
	case 8:
		return "Nine";
	case 9:
		return "Ten";
	case 10:
		return "Jack";
	case 11:
		return "Queen";
	case 12:
		return "King";
	case 13:
		return "Ace";
	default:
		return "Undefined Rank";
	}
}

bool PokerHand::operator==(const PokerHand& otherHand) const
{
	return this->handStrength == otherHand.handStrength && this->kicker == otherHand.kicker;
}

bool PokerHand::operator!=(const PokerHand& otherHand) const
{
	return !(*this == otherHand);
}

bool PokerHand::operator<(const PokerHand& otherHand) const
{
	return std::tie(this->handStrength, this->kicker) < std::tie(otherHand.handStrength, otherHand.kicker);
}

bool PokerHand::operator>(const PokerHand& otherHand) const
{
	return otherHand < *this;
}

bool PokerHand::operator<=(const PokerHand& otherHand) const
{
	return !(*this > otherHand);
}

bool PokerHand::operator>= (const PokerHand& otherHand) const
{
	return !(*this < otherHand);
}

void PokerHand::initializeRankTotal()
{
	std::size_t arrayIndex = 0;
	for (std::size_t i = rankTotal.size(); i > 0; i--, arrayIndex++)
	{
		rankTotal[arrayIndex] = std::count_if(this->cards.begin(), this->cards.end(), 
			[&](const Card& card) { return card.getRank() == static_cast<Card::Rank>(i); });
	}
}
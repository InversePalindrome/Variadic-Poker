/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - PlayState.hpp
InversePalindrome.com
*/


#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include "GameState.hpp"
#include "TextureManager.hpp"
#include "Dealer.hpp"
#include "PokerAI.hpp"


class StateStack;

class PlayState : public GameState
{
public:
    PlayState(StateStack& states, Data& data);

    virtual void processEvent() override;
    virtual void update(sf::Time deltaTime) override;
    virtual void draw() override;

private:
    struct PlayerItem
    {
        PlayerItem(const std::string& name, sfg::Label::Ptr label);

        std::string name;
        sfg::Label::Ptr label;
        std::vector<sf::Sprite> hiddenHoleCards;
    };

    sf::Sprite background;
    sfg::Box::Ptr actionFrame;
    sfg::Scrollbar::Ptr betBar;
    sfg::Scale::Ptr betScale;
    sfg::Adjustment::Ptr betAdjustment;
    sfg::Button::Ptr menuButton;
    sfg::Button::Ptr foldButton;
    sfg::Button::Ptr callButton;
    sfg::Button::Ptr betButton;
    sfg::Entry::Ptr betEntry;
    sfg::Label::Ptr tableLabel;
    sfg::Label::Ptr potLabel;
    std::vector<PlayerItem> playerItems;
    std::unordered_map<std::string, sf::Sprite> cardSprites;

    Dealer dealer;
    PokerAI pokerAI;
    const std::string mainPlayerName;
    bool mainPlayerActed;

    void executePokerGame();
    void transitionToMenu();
    void processBet();
    void positionPlayerItems();
    void adjustBetEntry();

    void updatePlayerItems();
    void updatePotLabel();

    void assignCardTextures();

    void drawCommunityCards();
    void drawHoleCards();
};

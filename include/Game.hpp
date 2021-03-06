/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - Game.hpp
InversePalindrome.com
*/


#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFGUI/SFGUI.hpp>
#include "StateStack.hpp"
#include "TextureManager.hpp"
#include "PokerTable.hpp"


class Game
{
public:
    Game();
    void run();

private:
    StateStack states;
    GameState::Data data;
    sf::RenderWindow window;
    sfg::SFGUI sfgui;
    TextureManager textures;
    PokerTable pokerTable;
};

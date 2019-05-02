/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - TableSelectState.cpp
InversePalindrome.com
*/


#include <memory>
#include <SFML/Window/Event.hpp>
#include "StateStack.hpp"
#include "GameSelectState.hpp"
#include "MenuState.hpp"
#include "PlayState.hpp"
#include "PokerTable.hpp"
#include "Player.hpp"


GameSelectState::GameSelectState(StateStack& states, Data& data) :
    GameState(states, data),
    selectionWindow(),
    selectionFrame(),
    tableSizeOptions(),
    menuButton(),
    doneButton(),
    bigBlindEntry(),
    anteEntry(),
    playerNameEntry(),
    playerStackEntry(),
    tableSizeLabel(),
    bigBlindLabel(),
    anteLabel(),
    playerNameLabel(),
    playerStackLabel()
{
    data.textures.loadTexture("TableSelectBackground", "Resources/TableSelectBackground.jpg");
    const auto& backgroundTexture = data.textures.getTexture("TableSelectBackground");

    background.setTexture(backgroundTexture);
    background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);
    background.setPosition(data.window.getSize().x / 2.f, data.window.getSize().y / 2.f);
    background.setScale(static_cast<float>(data.window.getSize().x) / backgroundTexture.getSize().x, static_cast<float>(data.window.getSize().y) / backgroundTexture.getSize().y);

    menuButton = sfg::Button::Create("  Menu  ");
    menuButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

    doneButton = sfg::Button::Create("  Done  ");
    doneButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToPlay(); });

    tableSizeLabel = sfg::Label::Create("Table Size:");

    tableSizeOptions = sfg::ComboBox::Create();
    tableSizeOptions->AppendItem("Heads-Up");
    tableSizeOptions->AppendItem("6-Max");
    tableSizeOptions->AppendItem("Full-Ring");

    bigBlindLabel = sfg::Label::Create("Big Blind:");
    bigBlindEntry = sfg::Entry::Create();
    bigBlindEntry->SetRequisition(sf::Vector2f(80.f, 0.f));

    anteLabel = sfg::Label::Create("Ante:");
    anteEntry = sfg::Entry::Create();
    anteEntry->SetRequisition(sf::Vector2f(80.f, 0.f));

    playerNameLabel = sfg::Label::Create("Player Name:");
    playerNameEntry = sfg::Entry::Create();
    playerNameEntry->SetRequisition(sf::Vector2f(80.f, 0.f));

    playerStackLabel = sfg::Label::Create("  Starting Stack:  ");
    playerStackEntry = sfg::Entry::Create();
    playerStackEntry->SetRequisition(sf::Vector2f(80.f, 0.f));

    selectionFrame = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
    selectionFrame->Pack(playerNameLabel);
    selectionFrame->Pack(playerNameEntry);
    selectionFrame->Pack(tableSizeLabel);
    selectionFrame->Pack(tableSizeOptions);
    selectionFrame->Pack(bigBlindLabel);
    selectionFrame->Pack(bigBlindEntry);
    selectionFrame->Pack(anteLabel);
    selectionFrame->Pack(anteEntry);
    selectionFrame->Pack(playerStackLabel);
    selectionFrame->Pack(playerStackEntry);
    selectionFrame->Pack(doneButton);

    selectionWindow = sfg::Window::Create();
    selectionWindow->SetTitle("Game Selection");
    selectionWindow->Add(selectionFrame);
    selectionWindow->SetPosition(sf::Vector2f(data.window.getSize().x / 2.4f, data.window.getSize().y / 3.1f));

    hud.Add(menuButton);
    hud.Add(selectionWindow);
}

void GameSelectState::processEvent()
{
    sf::Event event;

    while (this->data.window.pollEvent(event))
    {
        this->hud.HandleEvent(event);

        switch (event.type)
        {
        case sf::Event::Closed:
            this->data.window.close();
            break;
        }
    }
}

void GameSelectState::update(sf::Time deltaTime)
{
    this->hud.Update(static_cast<float>(deltaTime.asMicroseconds()));
}

void GameSelectState::draw()
{
    this->data.window.clear(sf::Color::Black);
    this->data.window.draw(this->background);
    this->data.sfgui.Display(this->data.window);
}

void GameSelectState::transitionToMenu()
{
    this->menuButton->Show(false);
    this->selectionWindow->Show(false);
    this->states.popState();
}

void GameSelectState::transitionToPlay()
{
    this->menuButton->Show(false);
    this->selectionWindow->Show(false);

    std::string name = this->playerNameEntry->GetText().toAnsiString();
    if (name.empty())
    {
        name = "Player";
    }

    std::size_t bigBlind;
    try
    {
        bigBlind = static_cast<std::size_t>(std::stoull(this->bigBlindEntry->GetText().toAnsiString()));
    }
    catch (const std::invalid_argument & e)
    {
        bigBlind = 10;
    }

    std::size_t ante;
    try
    {
        ante = static_cast<std::size_t>(std::stoull(this->anteEntry->GetText().toAnsiString()));
    }
    catch (const std::invalid_argument & e)
    {
        ante = 0;
    }

    std::size_t startingStack;
    try
    {
        startingStack = static_cast<std::size_t>(std::stoull(this->playerStackEntry->GetText().toAnsiString()));
    }
    catch (const std::invalid_argument & e)
    {
        startingStack = 1000;
    }

    std::size_t tableSize = 2;

    switch (this->tableSizeOptions->GetSelectedItem())
    {
    case 0:
        tableSize = 2;
        break;
    case 1:
        tableSize = 6;
        break;
    case 2:
        tableSize = 9;
    }

    this->setPokerTable(name, tableSize, bigBlind, ante, startingStack);
    this->states.popState();
    this->states.pushState(StateStack::PlayState);
}

void GameSelectState::setPokerTable(const std::string & playerName, std::size_t tableSize, std::size_t bigBlind, std::size_t ante, std::size_t startingStack)
{
    for (std::size_t i = 1; i < tableSize; i++)
    {
        this->data.pokerTable.addPlayer(Player("Player_AI" + std::to_string(i), startingStack));
    }

    std::size_t mainPlayerPosition = tableSize > 2 ? 2 : 0;

    this->data.pokerTable.insertPlayer(Player(playerName, startingStack), mainPlayerPosition);

    this->data.pokerTable.setBigBlind(bigBlind);
    this->data.pokerTable.setAnte(ante);
}
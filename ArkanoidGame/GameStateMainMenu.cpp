#include "GameStateMainMenu.h"
#include "GameStatePlaying.h"
#include "GameStateRecords.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <cassert>
#include <iostream>

namespace ArkanoidGame
{
    void MainMenuState::onEnter()
    {
        std::cout << "[MainMenuState] onEnter() called" << std::endl;
        
        bool fontLoaded = font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf");
        std::cout << "[MainMenuState] Font loaded: " << (fontLoaded ? "YES" : "NO") << std::endl;

        // Start Game item
        MenuItem startGame;
        startGame.text.setString("Start Game");
        if (fontLoaded) startGame.text.setFont(font);
        startGame.text.setCharacterSize(24);
        startGame.onPressCallback = [](MenuItem&) {
            std::cout << "[MainMenuState] Start Game selected" << std::endl;
            try {
                Game& game = Application::Instance().GetGame();
                game.switchState(std::make_unique<PlayingState>(SCREEN_WIDTH, SCREEN_HEIGHT));
            }
            catch (const std::exception& e) {
                std::cout << "[MainMenuState] Exception in Start Game: " << e.what() << std::endl;
            }
        };

        // Options - Infinite Lives
        bool isInfiniteLives = IsEnableOptions(Application::Instance().GetGame(), GameOptions::InfiniteLives);
        MenuItem optionsInfiniteLivesItem;
        optionsInfiniteLivesItem.text.setString("Infinite Lives: " + std::string(isInfiniteLives ? "On" : "Off"));
        if (fontLoaded) optionsInfiniteLivesItem.text.setFont(font);
        optionsInfiniteLivesItem.text.setCharacterSize(24);
        optionsInfiniteLivesItem.onPressCallback = [](MenuItem& item) {
            Game& game = Application::Instance().GetGame();
            game.setOptions((GameOptions)((std::uint8_t)game.getOptions() ^ (std::uint8_t)GameOptions::InfiniteLives));
            bool isInfiniteLives = IsEnableOptions(game, GameOptions::InfiniteLives);
            item.text.setString("Infinite Lives: " + std::string(isInfiniteLives ? "On" : "Off"));
        };

        // Options - Unlimited Balls
        bool isUnlimitedBalls = IsEnableOptions(Application::Instance().GetGame(), GameOptions::UnlimitedBalls);
        MenuItem optionsUnlimitedBallsItem;
        optionsUnlimitedBallsItem.text.setString("Unlimited Balls: " + std::string(isUnlimitedBalls ? "On" : "Off"));
        if (fontLoaded) optionsUnlimitedBallsItem.text.setFont(font);
        optionsUnlimitedBallsItem.text.setCharacterSize(24);
        optionsUnlimitedBallsItem.onPressCallback = [](MenuItem& item) {
            Game& game = Application::Instance().GetGame();
            game.setOptions((GameOptions)((std::uint8_t)game.getOptions() ^ (std::uint8_t)GameOptions::UnlimitedBalls));
            bool isUnlimitedBalls = IsEnableOptions(game, GameOptions::UnlimitedBalls);
            item.text.setString("Unlimited Balls: " + std::string(isUnlimitedBalls ? "On" : "Off"));
        };

        // Options submenu
        MenuItem options;
        options.text.setString("Options");
        if (fontLoaded) options.text.setFont(font);
        options.text.setCharacterSize(24);
        options.hintText.setString("Options");
        if (fontLoaded) options.hintText.setFont(font);
        options.hintText.setCharacterSize(48);
        options.hintText.setFillColor(sf::Color::Red);
        options.childrenOrientation = Orientation::Vertical;
        options.childrenAlignment = Alignment::Middle;
        options.childrenSpacing = 10.f;
        options.childrens.push_back(optionsInfiniteLivesItem);
        options.childrens.push_back(optionsUnlimitedBallsItem);

        // Records item
        MenuItem recordsItem;
        recordsItem.text.setString("Records");
        if (fontLoaded) recordsItem.text.setFont(font);
        recordsItem.text.setCharacterSize(24);
        recordsItem.onPressCallback = [](MenuItem&) {
            std::cout << "[MainMenuState] Records selected" << std::endl;
            Game& game = Application::Instance().GetGame();
            // Replace current state with Records state (not overlay)
            game.switchState(std::make_unique<RecordsState>());
        };

        // Exit Game - Yes/No submenu
        MenuItem yesItem;
        yesItem.text.setString("Yes");
        if (fontLoaded) yesItem.text.setFont(font);
        yesItem.text.setCharacterSize(24);
        yesItem.onPressCallback = [](MenuItem&) {
            std::cout << "[MainMenuState] Exit confirmed" << std::endl;
            try {
                Application::Instance().GetWindow().close();
            }
            catch (const std::exception& e) {
                std::cout << "[MainMenuState] Exception in Exit: " << e.what() << std::endl;
            }
        };

        MenuItem noItem;
        noItem.text.setString("No");
        if (fontLoaded) noItem.text.setFont(font);
        noItem.text.setCharacterSize(24);
        // Make 'No' actively return to parent menu when pressed
        noItem.onPressCallback = [this](MenuItem&) {
            menu.GoBack();
        };

        MenuItem exitGameItem;
        exitGameItem.text.setString("Exit Game");
        if (fontLoaded) exitGameItem.text.setFont(font);
        exitGameItem.text.setCharacterSize(24);
        exitGameItem.hintText.setString("Are you sure?");
        if (fontLoaded) exitGameItem.hintText.setFont(font);
        exitGameItem.hintText.setCharacterSize(48);
        exitGameItem.hintText.setFillColor(sf::Color::Red);
        exitGameItem.childrenOrientation = Orientation::Horizontal;
        exitGameItem.childrenAlignment = Alignment::Middle;
        exitGameItem.childrenSpacing = 10.f;
        exitGameItem.childrens.push_back(yesItem);
        exitGameItem.childrens.push_back(noItem);

        // Main menu
        MenuItem mainMenu;
        mainMenu.hintText.setString("Arkanoid Game");
        if (fontLoaded) mainMenu.hintText.setFont(font);
        mainMenu.hintText.setCharacterSize(48);
        mainMenu.hintText.setFillColor(sf::Color::Red);
        mainMenu.childrenOrientation = Orientation::Vertical;
        mainMenu.childrenAlignment = Alignment::Middle;
        mainMenu.childrenSpacing = 10.f;
        mainMenu.childrens.push_back(startGame);
        mainMenu.childrens.push_back(options);
        mainMenu.childrens.push_back(recordsItem);
        mainMenu.childrens.push_back(exitGameItem);

        std::cout << "[MainMenuState] Initializing menu with " << mainMenu.childrens.size() << " items..." << std::endl;
        try {
            menu.Init(mainMenu);
            std::cout << "[MainMenuState] Menu initialized successfully" << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "[MainMenuState] Exception in menu.Init(): " << e.what() << std::endl;
            throw;
        }
        
        std::cout << "[MainMenuState] onEnter() complete" << std::endl;
    }

    void MainMenuState::onExit()
    {
        std::cout << "[MainMenuState] onExit() called" << std::endl;
    }

    void MainMenuState::handleEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                menu.GoBack();
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                menu.PressOnSelectedItem();
            }

            Orientation orientation = menu.GetCurrentContext().childrenOrientation;
            if ((orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Up) ||
                (orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Left))
            {
                menu.SwitchToPreviousMenuItem();
            }
            else if ((orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Down) ||
                     (orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Right))
            {
                menu.SwitchToNextMenuItem();
            }
        }
    }

    void MainMenuState::update(float /*timeDelta*/)
    {
        // Menu doesn't need time-based updates
    }

    void MainMenuState::draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

        sf::Text* hintText = &menu.GetCurrentContext().hintText;
        hintText->setOrigin(GetTextOrigin(*hintText, { 0.5f, 0.f }));
        hintText->setPosition(viewSize.x / 2.f, 150.f);
        window.draw(*hintText);

        menu.Draw(window, viewSize / 2.f, { 0.5f, 0.f });
    }
}
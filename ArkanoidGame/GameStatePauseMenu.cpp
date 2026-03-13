#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <cassert>
#include <iostream>

namespace ArkanoidGame
{
    void PauseMenuState::onEnter()
    {
        std::cout << "[PauseMenuState] onEnter() called" << std::endl;
        
        bool fontLoaded = font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf");
        std::cout << "[PauseMenuState] Font loaded: " << (fontLoaded ? "YES" : "NO") << std::endl;

        background.setFillColor(sf::Color(0, 0, 0, 128));

        titleText.setString("Pause");
        if (fontLoaded) titleText.setFont(font);
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::Red);

        MenuItem resumeItem;
        resumeItem.text.setString("Return to game");
        if (fontLoaded) resumeItem.text.setFont(font);
        resumeItem.text.setCharacterSize(24);
        resumeItem.onPressCallback = [](MenuItem&) {
            std::cout << "[PauseMenuState] Resume selected" << std::endl;
            // Pop pause menu to return to game
            Application::Instance().GetGame().popState();
        };

        MenuItem exitItem;
        exitItem.text.setString("Exit to main menu");
        if (fontLoaded) exitItem.text.setFont(font);
        exitItem.text.setCharacterSize(24);
        exitItem.onPressCallback = [](MenuItem&) {
            std::cout << "[PauseMenuState] Exit to menu selected" << std::endl;
            Game& game = Application::Instance().GetGame();
            // Pop pause menu
            game.popState();
            // Switch to main menu (replaces playing state)
            game.switchState(std::make_unique<MainMenuState>());
        };

        MenuItem pauseMenu;
        pauseMenu.childrenOrientation = Orientation::Vertical;
        pauseMenu.childrenAlignment = Alignment::Middle;
        pauseMenu.childrens.push_back(resumeItem);
        pauseMenu.childrens.push_back(exitItem);

        std::cout << "[PauseMenuState] Initializing menu..." << std::endl;
        menu.Init(pauseMenu);
        
        std::cout << "[PauseMenuState] onEnter() complete" << std::endl;
    }

    void PauseMenuState::onExit()
    {
        std::cout << "[PauseMenuState] onExit() called" << std::endl;
        // Resources cleaned up by destructor (RAII)
    }

    void PauseMenuState::handleEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                std::cout << "[PauseMenuState] Escape - resuming game" << std::endl;
                // ESC resumes game
                Application::Instance().GetGame().popState();
            }

            if (event.key.code == sf::Keyboard::Enter)
            {
                menu.PressOnSelectedItem();
            }

            if (event.key.code == sf::Keyboard::Up)
            {
                menu.SwitchToPreviousMenuItem();
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                menu.SwitchToNextMenuItem();
            }
        }
    }

    void PauseMenuState::update(float /*timeDelta*/)
    {
        // Menu doesn't need time-based updates
    }

    void PauseMenuState::draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

        background.setSize(viewSize);
        window.draw(background);

        titleText.setOrigin(GetTextOrigin(titleText, { 0.5f, 0.f }));
        titleText.setPosition(viewSize.x / 2.f, 100);
        window.draw(titleText);

        menu.Draw(window, window.getView().getCenter(), { 0.5f, 0.f });
    }
}
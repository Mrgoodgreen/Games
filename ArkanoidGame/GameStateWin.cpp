#include "GameStateWin.h"
#include "GameStatePlaying.h"
#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <iostream>

namespace ArkanoidGame
{
    void WinState::onEnter()
    {
        std::cout << "[WinState] onEnter() called" << std::endl;

        fontLoaded = font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf");

        sf::Color backgroundColor = sf::Color::Black;
        backgroundColor.a = 180;
        background.setFillColor(backgroundColor);

        if (fontLoaded)
        {
            titleText.setFont(font);
            titleText.setCharacterSize(48);
            titleText.setStyle(sf::Text::Bold);
            titleText.setFillColor(sf::Color::Green);
            titleText.setString("YOU WIN!");

            questionText.setFont(font);
            questionText.setCharacterSize(28);
            questionText.setFillColor(sf::Color::White);
            questionText.setString("Play again?");

            yesText.setFont(font);
            yesText.setCharacterSize(28);
            yesText.setString("Yes");

            noText.setFont(font);
            noText.setCharacterSize(28);
            noText.setString("No");
        }

        selectYes = true;
        updateSelectionVisuals();
    }

    void WinState::onExit()
    {
        std::cout << "[WinState] onExit() called" << std::endl;
    }

    void WinState::handleEvent(const sf::Event& event)
    {
        if (event.type != sf::Event::KeyPressed)
            return;

        if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
        {
            selectYes = true;
            updateSelectionVisuals();
        }
        else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
        {
            selectYes = false;
            updateSelectionVisuals();
        }
        else if (event.key.code == sf::Keyboard::Y)
        {
            selectYes = true;
            updateSelectionVisuals();
            Game& game = Application::Instance().GetGame();
            game.switchState(std::make_unique<PlayingState>(SCREEN_WIDTH, SCREEN_HEIGHT));
        }
        else if (event.key.code == sf::Keyboard::N)
        {
            selectYes = false;
            updateSelectionVisuals();
            Game& game = Application::Instance().GetGame();
            game.switchState(std::make_unique<MainMenuState>());
        }
        else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space)
        {
            Game& game = Application::Instance().GetGame();
            if (selectYes)
            {
                game.switchState(std::make_unique<PlayingState>(SCREEN_WIDTH, SCREEN_HEIGHT));
            }
            else
            {
                game.switchState(std::make_unique<MainMenuState>());
            }
        }
    }

    void WinState::update(float /*timeDelta*/)
    {
    }

    void WinState::draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = window.getView().getSize();

        background.setOrigin(0.f, 0.f);
        background.setSize(viewSize);
        window.draw(background);

        if (!fontLoaded)
            return;

        titleText.setOrigin(GetTextOrigin(titleText, { 0.5f, 1.f }));
        titleText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f - 70.f);
        window.draw(titleText);

        questionText.setOrigin(GetTextOrigin(questionText, { 0.5f, 0.5f }));
        questionText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f - 10.f);
        window.draw(questionText);

        yesText.setOrigin(GetTextOrigin(yesText, { 0.5f, 0.f }));
        yesText.setPosition(viewSize.x / 2.f - 60.f, viewSize.y / 2.f + 30.f);
        window.draw(yesText);

        noText.setOrigin(GetTextOrigin(noText, { 0.5f, 0.f }));
        noText.setPosition(viewSize.x / 2.f + 60.f, viewSize.y / 2.f + 30.f);
        window.draw(noText);
    }

    void WinState::updateSelectionVisuals()
    {
        yesText.setFillColor(selectYes ? sf::Color::Yellow : sf::Color::White);
        noText.setFillColor(selectYes ? sf::Color::White : sf::Color::Yellow);
    }
}

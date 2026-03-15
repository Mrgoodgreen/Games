#include "GameStateWin.h"
#include "GameStatePlaying.h"
#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <iostream>
#include <sstream>
#include <map>

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

            Game& game = Application::Instance().GetGame();
            finalScoreText.setFont(font);
            finalScoreText.setCharacterSize(28);
            finalScoreText.setFillColor(sf::Color::Yellow);
            finalScoreText.setString("Final score: " + std::to_string(game.getLastScore()));

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

            recordsTableTexts.clear();
            recordsTableTexts.reserve(MAX_RECORDS_TABLE_SIZE);

            std::multimap<int, std::string> sortedRecords;
            for (const auto& item : game.getRecordsTable())
            {
                sortedRecords.insert(std::make_pair(item.second, item.first));
            }

            int index = 1;
            for (auto it = sortedRecords.rbegin(); it != sortedRecords.rend() && index <= MAX_RECORDS_TABLE_SIZE; ++it, ++index)
            {
                sf::Text record;
                record.setFont(font);
                record.setCharacterSize(20);
                record.setFillColor(sf::Color::White);

                std::stringstream stream;
                stream << index << ". " << it->second << ": " << it->first;
                record.setString(stream.str());
                recordsTableTexts.push_back(record);
            }
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
        titleText.setPosition(viewSize.x / 2.f, 90.f);
        window.draw(titleText);

        finalScoreText.setOrigin(GetTextOrigin(finalScoreText, { 0.5f, 0.f }));
        finalScoreText.setPosition(viewSize.x / 2.f, 110.f);
        window.draw(finalScoreText);

        float recordsTop = 170.f;
        for (std::size_t i = 0; i < recordsTableTexts.size(); ++i)
        {
            recordsTableTexts[i].setOrigin(GetTextOrigin(recordsTableTexts[i], { 0.5f, 0.f }));
            recordsTableTexts[i].setPosition(viewSize.x / 2.f, recordsTop + static_cast<float>(i) * 28.f);
            window.draw(recordsTableTexts[i]);
        }

        questionText.setOrigin(GetTextOrigin(questionText, { 0.5f, 0.5f }));
        questionText.setPosition(viewSize.x / 2.f, viewSize.y - 120.f);
        window.draw(questionText);

        yesText.setOrigin(GetTextOrigin(yesText, { 0.5f, 0.f }));
        yesText.setPosition(viewSize.x / 2.f - 60.f, viewSize.y - 90.f);
        window.draw(yesText);

        noText.setOrigin(GetTextOrigin(noText, { 0.5f, 0.f }));
        noText.setPosition(viewSize.x / 2.f + 60.f, viewSize.y - 90.f);
        window.draw(noText);
    }

    void WinState::updateSelectionVisuals()
    {
        yesText.setFillColor(selectYes ? sf::Color::Yellow : sf::Color::White);
        noText.setFillColor(selectYes ? sf::Color::White : sf::Color::Yellow);
    }
}

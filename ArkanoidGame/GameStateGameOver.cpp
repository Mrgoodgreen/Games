#include "GameStateGameOver.h"
#include "GameStatePlaying.h"
#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <cassert>
#include <sstream>
#include <map>
#include <iostream>

namespace ArkanoidGame
{
    void GameOverState::onEnter()
    {
        std::cout << "[GameOverState] onEnter() called" << std::endl;
        
        bool fontLoaded = font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf");
        std::cout << "[GameOverState] Font loaded: " << (fontLoaded ? "YES" : "NO") << std::endl;

        timeSinceGameOver = 0.f;

        sf::Color backgroundColor = sf::Color::Black;
        backgroundColor.a = 200;
        background.setFillColor(backgroundColor);

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(48);
        gameOverText.setStyle(sf::Text::Bold);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString("GAME OVER");

        Game& game = Application::Instance().GetGame();

        finalScoreText.setFont(font);
        finalScoreText.setCharacterSize(28);
        finalScoreText.setFillColor(sf::Color::Yellow);
        finalScoreText.setString("Final score: " + std::to_string(game.getLastScore()));

        recordsTableTexts.clear();
        recordsTableTexts.reserve(MAX_RECORDS_TABLE_SIZE);

        std::multimap<int, std::string> sortedRecordsTable;
        int playerScore = 0;
        auto& records = game.getRecordsTable();
        auto itPlayer = records.find(PLAYER_NAME);
        if (itPlayer != records.end()) playerScore = itPlayer->second;

        for (const auto& item : records)
        {
            sortedRecordsTable.insert(std::make_pair(item.second, item.first));
        }

        auto it = sortedRecordsTable.rbegin();
        for (int i = 0; i < MAX_RECORDS_TABLE_SIZE && it != sortedRecordsTable.rend(); ++i, ++it)
        {
            recordsTableTexts.emplace_back();
            sf::Text& text = recordsTableTexts.back();

            std::stringstream sstream;
            sstream << i + 1 << ". " << it->second << ": " << it->first;
            text.setString(sstream.str());
            text.setFont(font);
            text.setFillColor(sf::Color::White);
            text.setCharacterSize(24);
        }

        hintText.setFont(font);
        hintText.setCharacterSize(24);
        hintText.setFillColor(sf::Color::White);
        hintText.setString("Press Space to restart\nEsc to exit to main menu");
        
        std::cout << "[GameOverState] onEnter() complete" << std::endl;
    }

    void GameOverState::onExit()
    {
        std::cout << "[GameOverState] onExit() called" << std::endl;
    }

    void GameOverState::handleEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space)
            {
                std::cout << "[GameOverState] Space - restarting game" << std::endl;
                Game& game = Application::Instance().GetGame();
                game.switchState(std::make_unique<PlayingState>(SCREEN_WIDTH, SCREEN_HEIGHT));
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                std::cout << "[GameOverState] Escape - back to menu" << std::endl;
                Game& game = Application::Instance().GetGame();
                game.switchState(std::make_unique<MainMenuState>());
            }
        }
    }

    void GameOverState::update(float timeDelta)
    {
        timeSinceGameOver += timeDelta;

        sf::Color gameOverTextColor = (int)timeSinceGameOver % 2 ? sf::Color::Red : sf::Color::Yellow;
        gameOverText.setFillColor(gameOverTextColor);
    }

    void GameOverState::draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = window.getView().getSize();

        background.setOrigin(0.f, 0.f);
        background.setSize(viewSize);
        window.draw(background);

        gameOverText.setOrigin(GetTextOrigin(gameOverText, { 0.5f, 1.f }));
        gameOverText.setPosition(viewSize.x / 2.f, 110.f);
        window.draw(gameOverText);

        finalScoreText.setOrigin(GetTextOrigin(finalScoreText, { 0.5f, 0.f }));
        finalScoreText.setPosition(viewSize.x / 2.f, 120.f);
        window.draw(finalScoreText);

        std::vector<sf::Text*> textsList;
        textsList.reserve(recordsTableTexts.size());
        for (auto& text : recordsTableTexts)
        {
            textsList.push_back(&text);
        }

        sf::Vector2f tablePosition = { viewSize.x / 2, 190.f };
        DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Min, tablePosition, { 0.5f, 0.f });

        hintText.setOrigin(GetTextOrigin(hintText, { 0.5f, 1.f }));
        hintText.setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
        window.draw(hintText);
    }
}
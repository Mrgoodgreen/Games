#include "GameStateGameOver.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <assert.h>
#include <sstream>
#include <map>

namespace ArkanoidGame
{
    void GameStateGameOverData::init()
    {
        assert(font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

        timeSinceGameOver = 0.f;

        sf::Color backgroundColor = sf::Color::Black;
        backgroundColor.a = 200;
        background.setFillColor(backgroundColor);

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(48);
        gameOverText.setStyle(sf::Text::Bold);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString("GAME OVER");

        recordsTableTexts.clear();
        recordsTableTexts.reserve(MAX_RECORDS_TABLE_SIZE);

        std::multimap<int, std::string> sortedRecordsTable;
        Game& game = Application::Instance().GetGame();
        int playerScore = 0;
        auto& records = game.getRecordsTable();
        auto itPlayer = records.find(PLAYER_NAME);
        if (itPlayer != records.end()) playerScore = itPlayer->second;

        for (const auto& item : records)
        {
            sortedRecordsTable.insert(std::make_pair(item.second, item.first));
        }

        bool isPlayerInTable = false;
        auto it = sortedRecordsTable.rbegin();
        for (int i = 0; i < MAX_RECORDS_TABLE_SIZE && it != sortedRecordsTable.rend(); ++i, ++it)
        {
            recordsTableTexts.emplace_back();
            sf::Text& text = recordsTableTexts.back();

            std::stringstream sstream;
            sstream << i + 1 << ". " << it->second << ": " << it->first;
            text.setString(sstream.str());
            text.setFont(font);
            text.setCharacterSize(24);
            if (it->second == PLAYER_NAME)
            {
                text.setFillColor(sf::Color::Green);
                isPlayerInTable = true;
            }
            else
            {
                text.setFillColor(sf::Color::White);
            }
        }

        if (!isPlayerInTable && !recordsTableTexts.empty())
        {
            sf::Text& text = recordsTableTexts.back();
            std::stringstream sstream;
            sstream << MAX_RECORDS_TABLE_SIZE << ". " << PLAYER_NAME << ": " << playerScore;
            text.setString(sstream.str());
            text.setFillColor(sf::Color::Green);
        }

        hintText.setFont(font);
        hintText.setCharacterSize(24);
        hintText.setFillColor(sf::Color::White);
        hintText.setString("Press Space to restart\nEsc to exit to main menu");
    }

    void GameStateGameOverData::shutdown()
    {
        // Resources are automatically released
    }

    void GameStateGameOverData::handleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space)
            {
                SwitchGameState(Application::Instance().GetGame(), GameStateType::Playing);
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                SwitchGameState(Application::Instance().GetGame(), GameStateType::MainMenu);
            }
        }
    }

    void GameStateGameOverData::update(float timeDelta)
    {
        timeSinceGameOver += timeDelta;

        sf::Color gameOverTextColor = (int)timeSinceGameOver % 2 ? sf::Color::Red : sf::Color::Yellow;
        gameOverText.setFillColor(gameOverTextColor);
    }

    void GameStateGameOverData::draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = window.getView().getSize();

        background.setOrigin(0.f, 0.f);
        background.setSize(viewSize);
        window.draw(background);

        gameOverText.setOrigin(GetTextOrigin(gameOverText, { 0.5f, 1.f }));
        gameOverText.setPosition(viewSize.x / 2.f, viewSize.y / 2 - 50.f);
        window.draw(gameOverText);

        std::vector<sf::Text*> textsList;
        textsList.reserve(recordsTableTexts.size());
        for (auto& text : recordsTableTexts)
        {
            textsList.push_back(&text);
        }

        sf::Vector2f tablePosition = { viewSize.x / 2, viewSize.y / 2.f };
        DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Min, tablePosition, { 0.5f, 0.f });

        hintText.setOrigin(GetTextOrigin(hintText, { 0.5f, 1.f }));
        hintText.setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
        window.draw(hintText);
    }

    // Free-function wrappers
    void InitGameStateGameOver(GameStateGameOverData& data) { data.init(); }
    void ShutdownGameStateGameOver(GameStateGameOverData& data) { data.shutdown(); }
    void HandleGameStateGameOverWindowEvent(GameStateGameOverData& data, const sf::Event& event) { data.handleWindowEvent(event); }
    void UpdateGameStateGameOver(GameStateGameOverData& data, float timeDelta) { data.update(timeDelta); }
    void DrawGameStateGameOver(GameStateGameOverData& data, sf::RenderWindow& window) { data.draw(window); }
}
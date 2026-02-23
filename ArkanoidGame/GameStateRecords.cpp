#include "GameStateRecords.h"
#include "Application.h"
#include "Text.h"
#include "Game.h"
#include "GameSettings.h"
#include <assert.h>
#include <sstream>
#include <map>

namespace ArkanoidGame
{
    void GameStateRecordsData::init()
    {
        assert(font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

        titleText.setString("RECORDS");
        titleText.setFont(font);
        titleText.setFillColor(sf::Color::Red);
        titleText.setCharacterSize(48);

        tableTexts.clear();
        tableTexts.reserve(MAX_RECORDS_TABLE_SIZE);

        Game& game = Application::Instance().GetGame();
        std::multimap<int, std::string> sortedRecordsTable;
        for (const auto& item : game.getRecordsTable())
        {
            sortedRecordsTable.insert(std::make_pair(item.second, item.first));
        }

        int playerScore = 0;
        auto itPlayer = game.getRecordsTable().find(PLAYER_NAME);
        if (itPlayer != game.getRecordsTable().end()) playerScore = itPlayer->second;

        auto it = sortedRecordsTable.rbegin();
        for (int i = 0; i < MAX_RECORDS_TABLE_SIZE && it != sortedRecordsTable.rend(); ++i, ++it)
        {
            tableTexts.emplace_back();
            sf::Text& text = tableTexts.back();

            std::stringstream sstream;
            sstream << i + 1 << ". " << it->second << ": " << it->first;
            text.setString(sstream.str());
            text.setFont(font);
            text.setFillColor(sf::Color::White);
            text.setCharacterSize(24);
        }

        if (!tableTexts.empty())
        {
            // If player is not in table, ensure last entry shows player's score
            bool found = false;
            for (auto& t : tableTexts) {
                if (t.getString().find(PLAYER_NAME) != std::string::npos) { found = true; break; }
            }
            if (!found) {
                sf::Text& text = tableTexts.back();
                std::stringstream sstream;
                sstream << MAX_RECORDS_TABLE_SIZE << ". " << PLAYER_NAME << ": " << playerScore;
                text.setString(sstream.str());
                text.setFillColor(sf::Color::Green);
            }
        }

        hintText.setString("Press ESC to return back to main menu");
        hintText.setFont(font);
        hintText.setFillColor(sf::Color::White);
        hintText.setCharacterSize(24);
    }

    void GameStateRecordsData::shutdown()
    {
        // resources freed automatically
    }

    void GameStateRecordsData::handleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                PopGameState(Application::Instance().GetGame());
            }
        }
    }

    void GameStateRecordsData::update(float /*timeDelta*/)
    {
    }

    void GameStateRecordsData::draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = window.getView().getSize();

        titleText.setOrigin(GetTextOrigin(titleText, { 0.5f, 0.f }));
        titleText.setPosition(viewSize.x / 2.f, 50.f);
        window.draw(titleText);

        std::vector<sf::Text*> textsList;
        textsList.reserve(tableTexts.size());
        for (auto& text : tableTexts)
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
    void InitGameStateRecords(GameStateRecordsData& data) { data.init(); }
    void ShutdownGameStateRecords(GameStateRecordsData& data) { data.shutdown(); }
    void HandleGameStateRecordsWindowEvent(GameStateRecordsData& data, const sf::Event& event) { data.handleWindowEvent(event); }
    void UpdateGameStateRecords(GameStateRecordsData& data, float timeDelta) { data.update(timeDelta); }
    void DrawGameStateRecords(GameStateRecordsData& data, sf::RenderWindow& window) { data.draw(window); }
}
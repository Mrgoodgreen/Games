#include "GameStateRecords.h"
#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <cassert>
#include <sstream>
#include <map>
#include <iostream>
#include <memory>

namespace ArkanoidGame
{
    void RecordsState::onEnter()
    {
        std::cout << "[RecordsState] onEnter() called" << std::endl;
        
        bool fontLoaded = font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf");
        std::cout << "[RecordsState] Font loaded: " << (fontLoaded ? "YES" : "NO") << std::endl;

        titleText.setString("RECORDS");
        if (fontLoaded) titleText.setFont(font);
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
            if (fontLoaded) text.setFont(font);
            text.setFillColor(sf::Color::White);
            text.setCharacterSize(24);
        }

        if (!tableTexts.empty())
        {
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
        if (fontLoaded) hintText.setFont(font);
        hintText.setFillColor(sf::Color::White);
        hintText.setCharacterSize(24);
        
        std::cout << "[RecordsState] onEnter() complete" << std::endl;
    }

    void RecordsState::onExit()
    {
        std::cout << "[RecordsState] onExit() called" << std::endl;
        // Resources cleaned up by destructor (RAII)
    }

    void RecordsState::handleEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                std::cout << "[RecordsState] Escape - back to menu" << std::endl;
                // If this RecordsState replaced the main menu (switchState), switch back to a fresh main menu.
                Game& game = Application::Instance().GetGame();
                if (game.stateStack.size() == 1) {
                    game.switchState(std::make_unique<MainMenuState>());
                } else {
                    // Otherwise, if it was pushed, simply pop to return to previous state
                    game.popState();
                }
            }
        }
    }

    void RecordsState::update(float /*timeDelta*/)
    {
        // Records state doesn't need updates
    }

    void RecordsState::draw(sf::RenderWindow& window)
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
}
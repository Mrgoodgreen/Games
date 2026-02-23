#pragma once
#include "SFML/Graphics.hpp"
#include "GameSettings.h"
#include <vector>

namespace ArkanoidGame
{
    class GameStateRecordsData
    {
    public:
        GameStateRecordsData() = default;
        ~GameStateRecordsData() = default;

        void init();
        void shutdown();
        void handleWindowEvent(const sf::Event& event);
        void update(float timeDelta);
        void draw(sf::RenderWindow& window);

    private:
        sf::Font font;
        sf::Text titleText;
        std::vector<sf::Text> tableTexts;
        sf::Text hintText;
    };

    void InitGameStateRecords(GameStateRecordsData& data);
    void ShutdownGameStateRecords(GameStateRecordsData& data);
    void HandleGameStateRecordsWindowEvent(GameStateRecordsData& data, const sf::Event& event);
    void UpdateGameStateRecords(GameStateRecordsData& data, float timeDelta);
    void DrawGameStateRecords(GameStateRecordsData& data, sf::RenderWindow& window);
}
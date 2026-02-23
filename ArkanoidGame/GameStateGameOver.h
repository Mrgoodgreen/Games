#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

namespace ArkanoidGame
{
    class GameStateGameOverData
    {
    public:
        GameStateGameOverData() = default;
        ~GameStateGameOverData() = default;

        void init();
        void shutdown();
        void handleWindowEvent(const sf::Event& event);
        void update(float timeDelta);
        void draw(sf::RenderWindow& window);

    private:
        sf::Font font;
        float timeSinceGameOver = 0.f;
        sf::RectangleShape background;
        sf::Text gameOverText;
        sf::Text hintText;
        std::vector<sf::Text> recordsTableTexts;
    };

    void InitGameStateGameOver(GameStateGameOverData& data);
    void ShutdownGameStateGameOver(GameStateGameOverData& data);
    void HandleGameStateGameOverWindowEvent(GameStateGameOverData& data, const sf::Event& event);
    void UpdateGameStateGameOver(GameStateGameOverData& data, float timeDelta);
    void DrawGameStateGameOver(GameStateGameOverData& data, sf::RenderWindow& window);
}
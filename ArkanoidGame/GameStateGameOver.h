#pragma once
#include "IGameState.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include <map>

namespace ArkanoidGame
{
    // Game over state - displays final score and records table
    class GameOverState : public IGameState
    {
    private:
        sf::Font font;
        float timeSinceGameOver = 0.f;
        sf::RectangleShape background;
        sf::Text gameOverText;
        sf::Text finalScoreText;
        sf::Text hintText;
        std::vector<sf::Text> recordsTableTexts;

    public:
        GameOverState() = default;
        ~GameOverState() = default;

        void onEnter() override;
        void onExit() override;
        void handleEvent(const sf::Event& event) override;
        void update(float timeDelta) override;
        void draw(sf::RenderWindow& window) override;
        const char* getStateName() const override { return "GameOverState"; }
    };
}
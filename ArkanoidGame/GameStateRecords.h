#pragma once
#include "IGameState.h"
#include "SFML/Graphics.hpp"
#include <vector>

namespace ArkanoidGame
{
    // Records/high scores display state
    class RecordsState : public IGameState
    {
    private:
        sf::Font font;
        sf::Text titleText;
        std::vector<sf::Text> tableTexts;
        sf::Text hintText;

    public:
        RecordsState() = default;
        ~RecordsState() = default;

        void onEnter() override;
        void onExit() override;
        void handleEvent(const sf::Event& event) override;
        void update(float timeDelta) override;
        void draw(sf::RenderWindow& window) override;
        const char* getStateName() const override { return "RecordsState"; }
    };
}
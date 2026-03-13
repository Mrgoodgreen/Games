#pragma once

#include "IGameState.h"
#include "GameSettings.h"
#include "SFML/Graphics.hpp"

namespace ArkanoidGame
{
    class WinState : public IGameState
    {
    private:
        sf::Font font;
        bool fontLoaded = false;

        sf::RectangleShape background;
        sf::Text titleText;
        sf::Text questionText;
        sf::Text yesText;
        sf::Text noText;

        bool selectYes = true;

    public:
        WinState() = default;
        ~WinState() = default;

        void onEnter() override;
        void onExit() override;
        void handleEvent(const sf::Event& event) override;
        void update(float timeDelta) override;
        void draw(sf::RenderWindow& window) override;
        const char* getStateName() const override { return "WinState"; }

    private:
        void updateSelectionVisuals();
    };
}

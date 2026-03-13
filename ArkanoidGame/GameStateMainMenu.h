#pragma once
#include "IGameState.h"
#include "Menu.h"
#include "SFML/Graphics.hpp"

namespace ArkanoidGame
{
    // Main menu state
    class MainMenuState : public IGameState
    {
    private:
        Menu menu;
        sf::Font font;

    public:
        MainMenuState() = default;
        ~MainMenuState() = default;

        void onEnter() override;
        void onExit() override;
        void handleEvent(const sf::Event& event) override;
        void update(float timeDelta) override;
        void draw(sf::RenderWindow& window) override;
        const char* getStateName() const override { return "MainMenuState"; }
    };
}
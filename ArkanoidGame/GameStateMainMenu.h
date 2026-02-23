#pragma once
#include "SFML/Graphics.hpp"
#include "Text.h"
#include "GameSettings.h"
#include "Menu.h"
#include <functional>

namespace ArkanoidGame
{
    class GameStateMainMenuData
    {
    public:
        GameStateMainMenuData() = default;
        ~GameStateMainMenuData() = default;

        void init();
        void shutdown();
        void handleWindowEvent(const sf::Event& event);
        void update(float timeDelta);
        void draw(sf::RenderWindow& window);

    private:
        Menu menu;
        sf::Font font;
    };

    void InitGameStateMainMenu(GameStateMainMenuData& data);
    void ShutdownGameStateMainMenu(GameStateMainMenuData& data);
    void HandleGameStateMainMenuWindowEvent(GameStateMainMenuData& data, const sf::Event& event);
    void UpdateGameStateMainMenu(GameStateMainMenuData& data, float timeDelta);
    void DrawGameStateMainMenu(GameStateMainMenuData& data, sf::RenderWindow& window);
}
#pragma once
#include "SFML/Graphics.hpp"
#include "Menu.h"

namespace ArkanoidGame
{
    class GameStatePauseMenuData
    {
    public:
        GameStatePauseMenuData() = default;
        ~GameStatePauseMenuData() = default;

        void init();
        void shutdown();
        void handleWindowEvent(const sf::Event& event);
        void update(float timeDelta);
        void draw(sf::RenderWindow& window);

    private:
        Menu menu;
        sf::Font font;
        sf::RectangleShape background;
        sf::Text titleText;
    };

    void InitGameStatePauseMenu(GameStatePauseMenuData& data);
    void ShutdownGameStatePauseMenu(GameStatePauseMenuData& data);
    void HandleGameStatePauseMenuWindowEvent(GameStatePauseMenuData& data, const sf::Event& event);
    void UpdateGameStatePauseMenu(GameStatePauseMenuData& data, float timeDelta);
    void DrawGameStatePauseMenu(GameStatePauseMenuData& data, sf::RenderWindow& window);
}
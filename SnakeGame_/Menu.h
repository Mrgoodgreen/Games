#pragma once
#include <SFML/Graphics.hpp>
#include "Utils.h"

class Menu
{
public:
    Menu();
    ~Menu();

    void DrawMainMenu(sf::RenderWindow& window, int selectedOption);
    void DrawDifficultyMenu(sf::RenderWindow& window, int selectedDifficulty);
    void DrawSettingsMenu(sf::RenderWindow& window, int selectedOption, bool soundEnabled, bool musicEnabled);
    void DrawPauseMenu(sf::RenderWindow& window, int selectedOption);
    void DrawCountdown(sf::RenderWindow& window, float timeRemaining);

    bool LoadFont();

private:
    sf::Font m_Font;
};

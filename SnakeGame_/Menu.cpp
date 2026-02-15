#include "Menu.h"
#include "GameConfig.h"
#include <cassert>

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::DrawMainMenu(sf::RenderWindow& window, int selectedOption)
{
    sf::Text titleText;
    titleText.setFont(m_Font);
    titleText.setString("SNAKE GAME");
    titleText.setCharacterSize(48);
    titleText.setFillColor(GameConfig::SelectedColor);
    titleText.setPosition(250.0f, 80.0f);
    window.draw(titleText);

    const char* menuItems[] = {
        "Start Game",
        "Difficulty",
        "Score Table",
        "Settings",
        "Exit"
    };

    for (int i = 0; i < 5; ++i)
    {
        sf::Text itemText;
        itemText.setFont(m_Font);
        itemText.setString(menuItems[i]);
        itemText.setCharacterSize(28);
        itemText.setFillColor(i == selectedOption ? GameConfig::SelectedColor : GameConfig::TextColor);
        itemText.setPosition(320.0f, 200.0f + i * 60.0f);
        window.draw(itemText);
    }

    sf::Text controlsText;
    controlsText.setFont(m_Font);
    controlsText.setString("Controls: W/S - Navigate, Enter - Select");
    controlsText.setCharacterSize(16);
    controlsText.setFillColor(sf::Color(150, 150, 150));
    controlsText.setPosition(220.0f, 550.0f);
    window.draw(controlsText);
}

void Menu::DrawDifficultyMenu(sf::RenderWindow& window, int selectedDifficulty)
{
    sf::Text titleText;
    titleText.setFont(m_Font);
    titleText.setString("SELECT DIFFICULTY");
    titleText.setCharacterSize(36);
    titleText.setFillColor(GameConfig::SelectedColor);
    titleText.setPosition(220.0f, 80.0f);
    window.draw(titleText);

    for (int i = 0; i < 5; ++i)
    {
        sf::Text itemText;
        itemText.setFont(m_Font);
        itemText.setString(GameConfig::DifficultyNames[i]);
        itemText.setCharacterSize(26);
        itemText.setFillColor(i == selectedDifficulty ? GameConfig::SelectedColor : GameConfig::TextColor);
        itemText.setPosition(240.0f, 180.0f + i * 60.0f);
        window.draw(itemText);
    }

    sf::Text backText;
    backText.setFont(m_Font);
    backText.setString("Press B to go back");
    backText.setCharacterSize(18);
    backText.setFillColor(sf::Color(150, 150, 150));
    backText.setPosition(290.0f, 520.0f);
    window.draw(backText);
}

void Menu::DrawSettingsMenu(sf::RenderWindow& window, int selectedOption, bool soundEnabled, bool musicEnabled)
{
    sf::Text titleText;
    titleText.setFont(m_Font);
    titleText.setString("SETTINGS");
    titleText.setCharacterSize(36);
    titleText.setFillColor(GameConfig::SelectedColor);
    titleText.setPosition(310.0f, 100.0f);
    window.draw(titleText);

    sf::Text soundText;
    soundText.setFont(m_Font);
    soundText.setString(std::string("Sound: ") + (soundEnabled ? "ON" : "OFF"));
    soundText.setCharacterSize(28);
    soundText.setFillColor(selectedOption == 0 ? GameConfig::SelectedColor : GameConfig::TextColor);
    soundText.setPosition(300.0f, 220.0f);
    window.draw(soundText);

    sf::Text musicText;
    musicText.setFont(m_Font);
    musicText.setString(std::string("Music: ") + (musicEnabled ? "ON" : "OFF"));
    musicText.setCharacterSize(28);
    musicText.setFillColor(selectedOption == 1 ? GameConfig::SelectedColor : GameConfig::TextColor);
    musicText.setPosition(300.0f, 280.0f);
    window.draw(musicText);

    sf::Text backText;
    backText.setFont(m_Font);
    backText.setString("Back");
    backText.setCharacterSize(28);
    backText.setFillColor(selectedOption == 2 ? GameConfig::SelectedColor : GameConfig::TextColor);
    backText.setPosition(360.0f, 340.0f);
    window.draw(backText);

    sf::Text controlsText;
    controlsText.setFont(m_Font);
    controlsText.setString("W/S - Navigate, Enter - Toggle/Select");
    controlsText.setCharacterSize(16);
    controlsText.setFillColor(sf::Color(150, 150, 150));
    controlsText.setPosition(240.0f, 520.0f);
    window.draw(controlsText);
}

void Menu::DrawPauseMenu(sf::RenderWindow& window, int selectedOption)
{
    sf::RectangleShape overlay(sf::Vector2f(
        static_cast<float>(GameConfig::WindowWidth),
        static_cast<float>(GameConfig::WindowHeight)
    ));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::RectangleShape popup(sf::Vector2f(300.0f, 200.0f));
    popup.setPosition(250.0f, 200.0f);
    popup.setFillColor(sf::Color(50, 50, 50));
    popup.setOutlineColor(sf::Color::White);
    popup.setOutlineThickness(2.0f);
    window.draw(popup);

    sf::Text pauseText;
    pauseText.setFont(m_Font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(32);
    pauseText.setFillColor(GameConfig::SelectedColor);
    pauseText.setPosition(320.0f, 230.0f);
    window.draw(pauseText);

    sf::Text continueText;
    continueText.setFont(m_Font);
    continueText.setString("Continue");
    continueText.setCharacterSize(24);
    continueText.setFillColor(selectedOption == 0 ? GameConfig::SelectedColor : GameConfig::TextColor);
    continueText.setPosition(320.0f, 290.0f);
    window.draw(continueText);

    sf::Text exitText;
    exitText.setFont(m_Font);
    exitText.setString("Exit to Menu");
    exitText.setCharacterSize(24);
    exitText.setFillColor(selectedOption == 1 ? GameConfig::SelectedColor : GameConfig::TextColor);
    exitText.setPosition(300.0f, 330.0f);
    window.draw(exitText);
}

void Menu::DrawCountdown(sf::RenderWindow& window, float timeRemaining)
{
    int countdown = static_cast<int>(timeRemaining) + 1;
    
    sf::Text countdownText;
    countdownText.setFont(m_Font);
    countdownText.setString(std::to_string(countdown));
    countdownText.setCharacterSize(72);
    countdownText.setFillColor(GameConfig::SelectedColor);
    
    sf::FloatRect textBounds = countdownText.getLocalBounds();
    countdownText.setPosition(
        (GameConfig::WindowWidth - textBounds.width) / 2.0f,
        (GameConfig::WindowHeight - textBounds.height) / 2.0f - 50.0f
    );
    
    window.draw(countdownText);
}

bool Menu::LoadFont()
{
    bool loaded = m_Font.loadFromFile(GameConfig::FontPath);
    assert(loaded && "Failed to load font");
    return loaded;
}

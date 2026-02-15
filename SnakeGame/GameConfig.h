#pragma once
#include <SFML/Graphics.hpp>

// Game Configuration Constants
namespace GameConfig
{
    // Window settings
    constexpr int WindowWidth = 800;
    constexpr int WindowHeight = 600;
    inline const char* WindowTitle = "Snake Game";

    // Game field settings
    constexpr int GridSize = 20; // Size of one grid cell in pixels
    constexpr int FieldWidth = 30; // Number of cells horizontally
    constexpr int FieldHeight = 25; // Number of cells vertically
    constexpr int FieldOffsetX = 50;
    constexpr int FieldOffsetY = 50;

    // Snake settings
    constexpr int InitialSnakeLength = 2;
    constexpr int SnakeGrowthAmount = 1; // How many segments to add when eating apple

    // Timing settings
    constexpr float StartDelay = 2.0f; // Seconds before game starts
    constexpr float PauseDelay = 2.0f; // Seconds after unpausing

    // Difficulty levels (speed in cells per second)
    constexpr float DifficultySpeed[5] = { 4.0f, 6.0f, 8.0f, 10.0f, 12.0f };
    constexpr int DifficultyPoints[5] = { 2, 4, 6, 8, 10 };
    inline const char* DifficultyNames[5] = {
        "Simple",
        "Harder than Simple",
        "Medium",
        "Easier than Hard",
        "Hard"
    };

    // Score table settings
    constexpr int MaxScoresInPopup = 5;
    constexpr int MaxScoresInTable = 10;
    constexpr int MaxNameLength = 20;

    // File paths
    inline const char* ScoreFilePath = "scores.dat";
    inline const char* FontPath = "Resources/Fonts/arial.ttf";

    // Texture paths
    inline const char* SnakeHeadTexturePath = "Resources/Textures/snake_head.png";
    inline const char* SnakeBodyTexturePath = "Resources/Textures/snake_body.png";
    inline const char* AppleTexturePath = "Resources/Textures/apple.png";

    // Sound paths
    inline const char* ButtonClickSoundPath = "Resources/Sounds/button_click.wav";
    inline const char* CollisionSoundPath = "Resources/Sounds/collision.wav";
    inline const char* GameStartSoundPath = "Resources/Sounds/game_start.wav";
    inline const char* GameOverSoundPath = "Resources/Sounds/game_over.wav";
    inline const char* EatAppleSoundPath = "Resources/Sounds/eat_apple.wav";
    inline const char* GameMusicPath = "Resources/Sounds/game_music.ogg";

    // Colors
    inline const sf::Color BackgroundColor = sf::Color(40, 40, 40);
    inline const sf::Color GridColor = sf::Color(60, 60, 60);
    inline const sf::Color WallColor = sf::Color(100, 100, 100);
    inline const sf::Color TextColor = sf::Color::White;
    inline const sf::Color SelectedColor = sf::Color::Green;
    inline const sf::Color ScoreColor = sf::Color::Yellow;
}
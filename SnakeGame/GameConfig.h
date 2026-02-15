#pragma once

// Game Configuration Constants
namespace GameConfig
{
    // Window settings
    const int WindowWidth = 800;
    const int WindowHeight = 600;
    const char* WindowTitle = "Snake Game";

    // Game field settings
    const int GridSize = 20; // Size of one grid cell in pixels
    const int FieldWidth = 30; // Number of cells horizontally
    const int FieldHeight = 25; // Number of cells vertically
    const int FieldOffsetX = 50;
    const int FieldOffsetY = 50;

    // Snake settings
    const int InitialSnakeLength = 2;
    const int SnakeGrowthAmount = 1; // How many segments to add when eating apple

    // Timing settings
    const float StartDelay = 2.0f; // Seconds before game starts
    const float PauseDelay = 2.0f; // Seconds after unpausing

    // Difficulty levels (speed in cells per second)
    const float DifficultySpeed[5] = { 4.0f, 6.0f, 8.0f, 10.0f, 12.0f };
    const int DifficultyPoints[5] = { 2, 4, 6, 8, 10 };
    const char* DifficultyNames[5] = {
        "Simple",
        "Harder than Simple",
        "Medium",
        "Easier than Hard",
        "Hard"
    };

    // Score table settings
    const int MaxScoresInPopup = 5;
    const int MaxScoresInTable = 10;
    const int MaxNameLength = 20;

    // File paths
    const char* ScoreFilePath = "scores.dat";
    const char* FontPath = "Resources/Fonts/arial.ttf";
    
    // Texture paths
    const char* SnakeHeadTexturePath = "Resources/Textures/snake_head.png";
    const char* SnakeBodyTexturePath = "Resources/Textures/snake_body.png";
    const char* AppleTexturePath = "Resources/Textures/apple.png";

    // Sound paths
    const char* ButtonClickSoundPath = "Resources/Sounds/button_click.wav";
    const char* CollisionSoundPath = "Resources/Sounds/collision.wav";
    const char* GameStartSoundPath = "Resources/Sounds/game_start.wav";
    const char* GameOverSoundPath = "Resources/Sounds/game_over.wav";
    const char* EatAppleSoundPath = "Resources/Sounds/eat_apple.wav";
    const char* GameMusicPath = "Resources/Sounds/game_music.ogg";

    // Colors
    const sf::Color BackgroundColor = sf::Color(40, 40, 40);
    const sf::Color GridColor = sf::Color(60, 60, 60);
    const sf::Color WallColor = sf::Color(100, 100, 100);
    const sf::Color TextColor = sf::Color::White;
    const sf::Color SelectedColor = sf::Color::Green;
    const sf::Color ScoreColor = sf::Color::Yellow;
}

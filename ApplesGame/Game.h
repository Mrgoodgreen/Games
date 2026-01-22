#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

#include "Player.h"
#include "Apple.h"
#include "Obstacle.h"
#include "UI.h"
#include "GameSettings.h"

namespace ApplesGame
{
    struct Record
    {
        std::string name;
        int score = 0;
    };

    struct GameState
    {
        Player player;

        // Dynamic apple array - FIXED: now using applesDefaultCount for allocation
        Apple* apples = nullptr;
        int applesDefaultCount = 20;    // Default count selected by user (restored on restart)
        int applesCount = 0;            // Current count on field (decreases in finite mode)

        // Obstacles
        Obstacle obstacles[MAX_OBSTACLES];
        int currentObstaclesCount = BASE_NUM_OBSTACLES;

        int numEatenApples = 0;
        int bestScore = 0;

        UIState uiState;

        bool isGameOver = false;
        float timeSinceGameOver = 0.f;

        // Resources
        sf::Font font;
        sf::Texture playerTexture;
        sf::Texture appleTexture;
        sf::Texture obstacleTexture;

        // Sounds
        sf::SoundBuffer eatAppleBuffer;
        sf::SoundBuffer hitBuffer;
        sf::Sound eatAppleSound;
        sf::Sound hitSound;

        // Game mode (bitwise mask) - FIXED: now explicitly set all flags
        unsigned int gameModeMask = 0;

        // Speed
        float playerBaseSpeed = INITIAL_SPEED;
        float playerAcceleration = ACCELERATION;

        // Leaderboard
        std::vector<Record> leaderboard;
        bool isLeaderboardInitialized = false;
    };

    void InitGame(GameState& gameState);
    void RestartGame(GameState& gameState);
    void HandleInput(GameState& gameState);
    void UpdateGame(GameState& gameState, float timeDelta);
    void DrawGame(GameState& gameState, sf::RenderWindow& window);
    void CleanupGame(GameState& gameState);
}
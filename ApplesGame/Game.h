#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Apple.h"
#include "Obstacle.h"
#include "UI.h"
#include "GameSettings.h"

namespace ApplesGame
{
    struct GameState
    {
        Player player;

        // динамический массив €блок
        Apple* apples = nullptr;
        int applesCount = 0;          // текущее количество на поле
        int applesTargetTotal = -1;   // -1 = бесконечно, иначе конечное число

        // преп€тстви€
        Obstacle obstacles[MAX_OBSTACLES];
        int currentObstaclesCount = BASE_NUM_OBSTACLES;

        int numEatenApples = 0;
        int bestScore = 0;            // лучший счЄт за сессию (можно позже сохранить в файл)

        UIState uiState;

        bool isGameOver = false;
        float timeSinceGameOver = 0.f;

        // –есурсы
        sf::Font font;
        sf::Texture playerTexture;
        sf::Texture appleTexture;
        sf::Texture obstacleTexture;

        // «вуки
        sf::SoundBuffer eatAppleBuffer;
        sf::SoundBuffer hitBuffer;
        sf::Sound eatAppleSound;
        sf::Sound hitSound;

        // битова€ маска режима игры
        unsigned int gameModeMask = 0;

        // скорость
        float playerBaseSpeed = INITIAL_SPEED;
        float playerAcceleration = ACCELERATION;
    };

    void InitGame(GameState& gameState);
    void RestartGame(GameState& gameState);
    void HandleInput(GameState& gameState);
    void UpdateGame(GameState& gameState, float timeDelta);
    void DrawGame(GameState& gameState, sf::RenderWindow& window);
}

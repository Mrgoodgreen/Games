#include "Game.h"
#include "Math.h"
#include <cassert>
#include <algorithm>

namespace ApplesGame
{
    void InitGame(GameState& gameState)
    {
        // ресурсы
        assert(gameState.playerTexture.loadFromFile(RESOURCES_PATH + "Pacman.png"));
        assert(gameState.appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png"));
        assert(gameState.obstacleTexture.loadFromFile(RESOURCES_PATH + "Obstacle.png"));
        assert(gameState.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

        // звуки
        assert(gameState.eatAppleBuffer.loadFromFile(RESOURCES_PATH + "Sounds/eat.wav"));
        assert(gameState.hitBuffer.loadFromFile(RESOURCES_PATH + "Sounds/hit.wav"));

        gameState.eatAppleSound.setBuffer(gameState.eatAppleBuffer);
        gameState.hitSound.setBuffer(gameState.hitBuffer);

        InitUI(gameState.uiState, gameState.font);

        // выделяем память под максимальное количество яблок один раз
        if (!gameState.apples)
        {
            gameState.apples = new Apple[MAX_APPLES];
        }

        RestartGame(gameState);
    }

    void RestartGame(GameState& gameState)
    {
        // безопасное значение, если что‑то не установили
        if (gameState.applesCount <= 0 || gameState.applesCount > MAX_APPLES)
            gameState.applesCount = 20;

        // препятствия: базовое значение
        gameState.currentObstaclesCount = BASE_NUM_OBSTACLES;

        // ускорение / отсутствие ускорения
        if (gameState.gameModeMask & MODE_SPEED_ACCEL_ON)
            gameState.playerAcceleration = ACCELERATION;
        else if (gameState.gameModeMask & MODE_SPEED_ACCEL_OFF)
            gameState.playerAcceleration = 0.f;
        else
            gameState.playerAcceleration = ACCELERATION;

        InitPlayer(gameState.player, gameState.playerTexture);
        gameState.player.speed = INITIAL_SPEED;

        // инициализация яблок
        for (int i = 0; i < gameState.applesCount; ++i)
        {
            InitApple(gameState.apples[i], gameState.appleTexture);
        }

        // инициализация препятствий
        for (int i = 0; i < gameState.currentObstaclesCount; ++i)
        {
            InitObstacle(gameState.obstacles[i], gameState.obstacleTexture);
        }

        gameState.numEatenApples = 0;
        gameState.isGameOver = false;
        gameState.timeSinceGameOver = 0.f;
    }

    void HandleInput(GameState& gameState)
    {
        if (gameState.isGameOver)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                RestartGame(gameState);
            }
            return;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            gameState.player.direction = PlayerDirection::Up;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            gameState.player.direction = PlayerDirection::Right;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            gameState.player.direction = PlayerDirection::Down;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            gameState.player.direction = PlayerDirection::Left;
        }
    }

    void UpdateGame(GameState& gameState, float timeDelta)
    {
        if (!gameState.isGameOver)
        {
            // движение игрока
            UpdatePlayer(gameState.player, timeDelta);

            // проверка столкновений с яблоками
            for (int i = 0; i < gameState.applesCount; ++i)
            {
                if (HasPlayerCollisionWithApple(gameState.player, gameState.apples[i]))
                {
                    // конечный / бесконечный режим
                    if (gameState.gameModeMask & MODE_APPLES_FINITE)
                    {
                        // удаляем яблоко: переносим последнее на его место
                        gameState.apples[i] = gameState.apples[gameState.applesCount - 1];
                        --gameState.applesCount;
                        --i; // обработать новое яблоко на этом индексе
                    }
                    else // бесконечные яблоки
                    {
                        InitApple(gameState.apples[i], gameState.appleTexture);
                    }

                    gameState.numEatenApples++;

                    // лучший счёт обновляем только по окончании игры, но
                    // можно и здесь, если хочешь «живой» best
                    if (gameState.numEatenApples > gameState.bestScore)
                        gameState.bestScore = gameState.numEatenApples;

                    // звук поедания
                    gameState.eatAppleSound.play();

                    // ускорение включено?
                    if (gameState.gameModeMask & MODE_SPEED_ACCEL_ON)
                    {
                        gameState.player.speed += gameState.playerAcceleration;
                    }

                    // если конечный режим и яблок не осталось – конец игры
                    if ((gameState.gameModeMask & MODE_APPLES_FINITE) &&
                        gameState.applesCount == 0)
                    {
                        gameState.isGameOver = true;
                        gameState.timeSinceGameOver = 0.f;
                        break;
                    }
                }
            }

            // столкновение с препятствиями
            for (int i = 0; i < gameState.currentObstaclesCount && !gameState.isGameOver; ++i)
            {
                if (HasPlayerCollisionWithObstacle(gameState.player, gameState.obstacles[i]))
                {
                    gameState.isGameOver = true;
                    gameState.timeSinceGameOver = 0.f;
                    gameState.hitSound.play();
                }
            }

            // столкновение со стенкой
            if (!gameState.isGameOver && HasPlayerCollisionWithScreenBorder(gameState.player))
            {
                gameState.isGameOver = true;
                gameState.timeSinceGameOver = 0.f;

                // звук удара
                gameState.hitSound.play();
            }
        }
        else
        {
            gameState.timeSinceGameOver += timeDelta;
        }

        UpdateUI(gameState.uiState, gameState, timeDelta);
    }

    void DrawGame(GameState& gameState, sf::RenderWindow& window)
    {
        DrawPlayer(gameState.player, window);

        for (int i = 0; i < gameState.applesCount; ++i)
        {
            DrawApple(gameState.apples[i], window);
        }

        // камни
        for (int i = 0; i < gameState.currentObstaclesCount; ++i)
        {
            DrawObstacle(gameState.obstacles[i], window);
        }

        DrawUI(gameState.uiState, window);
    }
}

#include "Game.h"
#include "Math.h"

#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace ApplesGame
{
    // ---------- вспомогательные данные для таблицы лидеров ----------

    const char* NAMES_POOL[] = {
        "Alice", "Bob", "Carol", "Dave", "Eve",
        "Frank", "Grace", "Heidi", "Ivan", "Judy"
    };
    const int NAMES_POOL_SIZE = sizeof(NAMES_POOL) / sizeof(NAMES_POOL[0]);

    // простая сортировка выбором по убыванию score (без std::sort)
    void SortLeaderboard(std::vector<Record>& board)
    {
        for (size_t i = 0; i + 1 < board.size(); ++i)
        {
            size_t bestIndex = i;
            for (size_t j = i + 1; j < board.size(); ++j)
            {
                if (board[j].score > board[bestIndex].score)
                    bestIndex = j;
            }
            if (bestIndex != i)
                std::swap(board[i], board[bestIndex]);
        }
    }

    void GenerateRandomLeaderboard(GameState& gameState, int count)
    {
        gameState.leaderboard.clear();
        count = std::max(5, std::min(count, 10)); // от 5 до 10 записей

        for (int i = 0; i < count; ++i)
        {
            Record r;
            r.name = NAMES_POOL[std::rand() % NAMES_POOL_SIZE];
            r.score = 20 + std::rand() % 120; // очки 20–139
            gameState.leaderboard.push_back(r);
        }

        gameState.isLeaderboardInitialized = true;
    }

    void UpdateLeaderboardWithPlayer(GameState& gameState)
    {
        Record playerRecord;
        playerRecord.name = "Player";
        playerRecord.score = gameState.numEatenApples;

        bool replaced = false;
        for (auto& r : gameState.leaderboard)
        {
            if (r.name == "Player")
            {
                r.score = playerRecord.score;
                replaced = true;
                break;
            }
        }
        if (!replaced)
        {
            gameState.leaderboard.push_back(playerRecord);
        }

        SortLeaderboard(gameState.leaderboard);
    }

    // ---------- основная логика ----------

    void InitGame(GameState& gameState)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

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
        // первая инициализация leaderboard – делаем новую таблицу
        if (!gameState.isLeaderboardInitialized)
        {
            GenerateRandomLeaderboard(gameState, 7); // например, 7 записей
        }

        if (gameState.applesCount <= 0 || gameState.applesCount > MAX_APPLES)
            gameState.applesCount = 20;

        gameState.currentObstaclesCount = BASE_NUM_OBSTACLES;

        if (gameState.gameModeMask & MODE_SPEED_ACCEL_ON)
            gameState.playerAcceleration = ACCELERATION;
        else if (gameState.gameModeMask & MODE_SPEED_ACCEL_OFF)
            gameState.playerAcceleration = 0.f;
        else
            gameState.playerAcceleration = ACCELERATION;

        InitPlayer(gameState.player, gameState.playerTexture);
        gameState.player.speed = INITIAL_SPEED;

        // яблоки
        for (int i = 0; i < gameState.applesCount; ++i)
        {
            InitApple(gameState.apples[i], gameState.appleTexture);
        }

        // препятствия
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
            UpdatePlayer(gameState.player, timeDelta);

            // столкновения с яблоками
            for (int i = 0; i < gameState.applesCount; ++i)
            {
                if (HasPlayerCollisionWithApple(gameState.player, gameState.apples[i]))
                {
                    if (gameState.gameModeMask & MODE_APPLES_FINITE)
                    {
                        gameState.apples[i] = gameState.apples[gameState.applesCount - 1];
                        --gameState.applesCount;
                        --i;
                    }
                    else
                    {
                        InitApple(gameState.apples[i], gameState.appleTexture);
                    }

                    gameState.numEatenApples++;

                    if (gameState.numEatenApples > gameState.bestScore)
                        gameState.bestScore = gameState.numEatenApples;

                    gameState.eatAppleSound.play();

                    if (gameState.gameModeMask & MODE_SPEED_ACCEL_ON)
                    {
                        gameState.player.speed += gameState.playerAcceleration;
                    }

                    if ((gameState.gameModeMask & MODE_APPLES_FINITE) &&
                        gameState.applesCount == 0)
                    {
                        gameState.isGameOver = true;
                        gameState.timeSinceGameOver = 0.f;
                        UpdateLeaderboardWithPlayer(gameState);
                        break;
                    }
                }
            }

            // столкновения с препятствиями
            for (int i = 0; i < gameState.currentObstaclesCount && !gameState.isGameOver; ++i)
            {
                if (HasPlayerCollisionWithObstacle(gameState.player, gameState.obstacles[i]))
                {
                    gameState.isGameOver = true;
                    gameState.timeSinceGameOver = 0.f;
                    gameState.hitSound.play();
                    UpdateLeaderboardWithPlayer(gameState);
                }
            }

            // столкновение со стенкой
            if (!gameState.isGameOver && HasPlayerCollisionWithScreenBorder(gameState.player))
            {
                gameState.isGameOver = true;
                gameState.timeSinceGameOver = 0.f;
                gameState.hitSound.play();
                UpdateLeaderboardWithPlayer(gameState);
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

        for (int i = 0; i < gameState.currentObstaclesCount; ++i)
        {
            DrawObstacle(gameState.obstacles[i], window);
        }

        DrawUI(gameState.uiState, window);
    }
}

#include "Game.h"
#include "Math.h"

#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace ApplesGame
{
    // Name pool for the leaderboard
    const char* NAMES_POOL[] = {
        "Alice", "Bob", "Carol", "Dave", "Eve",
        "Frank", "Grace", "Heidi", "Ivan", "Judy"
    };
    const int NAMES_POOL_SIZE = sizeof(NAMES_POOL) / sizeof(NAMES_POOL[0]);

    void SortLeaderboard(std::vector<Record>& board)
    {
        for (size_t i = 0; i + 1 < board.size(); ++i)
        {
            size_t bestIndex = i;
            for (size_t j = i + 1; j < board.size(); ++j)
            {
                if (board[j].score > board[bestIndex].score)
                {
                    bestIndex = j;
                }
            }

            if (bestIndex != i)
            {
                std::swap(board[i], board[bestIndex]);
            }
        }
    }

    void GenerateRandomLeaderboard(GameState& gameState, int count)
    {
        gameState.leaderboard.clear();

        count = std::max(5, std::min(count, 10));

        for (int i = 0; i < count; ++i)
        {
            Record r;
            r.name = NAMES_POOL[std::rand() % NAMES_POOL_SIZE];
            r.score = 20 + std::rand() % 120; // 20..139
            gameState.leaderboard.push_back(r);
        }

        gameState.isLeaderboardInitialized = true;
        SortLeaderboard(gameState.leaderboard);
    }

    void UpdateLeaderboardWithPlayer(GameState& gameState, const Record& playerRecord)
    {
        Record r = playerRecord;

        bool replaced = false;
        for (auto& rec : gameState.leaderboard)
        {
            if (rec.name == "Player")
            {
                if (r.score > rec.score)
                {
                    rec.score = r.score;
                }
                replaced = true;
                break;
            }
        }

        if (!replaced)
        {
            gameState.leaderboard.push_back(r);
        }

        SortLeaderboard(gameState.leaderboard);
    }

    void InitGame(GameState& gameState)
    {
        // srand has been removed from here – it is now called in main()

        assert(gameState.playerTexture.loadFromFile(RESOURCES_PATH + std::string("Pacman.png")));
        assert(gameState.appleTexture.loadFromFile(RESOURCES_PATH + std::string("Apple.png")));
        assert(gameState.obstacleTexture.loadFromFile(RESOURCES_PATH + std::string("Obstacle.png")));
        assert(gameState.font.loadFromFile(RESOURCES_PATH + std::string("Fonts/Roboto-Regular.ttf")));
        assert(gameState.eatAppleBuffer.loadFromFile(RESOURCES_PATH + std::string("Sounds/eat.wav")));
        assert(gameState.hitBuffer.loadFromFile(RESOURCES_PATH + std::string("Sounds/hit.wav")));

        gameState.eatAppleSound.setBuffer(gameState.eatAppleBuffer);
        gameState.hitSound.setBuffer(gameState.hitBuffer);

        InitUI(gameState.uiState, gameState.font);

        if (!gameState.apples)
        {
            gameState.apples = new Apple[MAX_APPLES];
        }

        RestartGame(gameState);
    }

    void RestartGame(GameState& gameState)
    {
        if (!gameState.isLeaderboardInitialized)
        {
            GenerateRandomLeaderboard(gameState, 7);
        }

        if (gameState.applesCount == 0)
        {
            gameState.applesCount = MAX_APPLES;
        }

        if (gameState.applesDefaultCount <= 0 || gameState.applesDefaultCount > MAX_APPLES)
        {
            gameState.applesDefaultCount = 20;
        }

        gameState.applesCount = gameState.applesDefaultCount;
        gameState.currentObstaclesCount = BASE_NUM_OBSTACLES;

        if (gameState.gameModeMask & MODE_SPEED_ACCEL_ON)
        {
            gameState.playerAcceleration = ACCELERATION;
        }
        else if (gameState.gameModeMask & MODE_SPEED_ACCEL_OFF)
        {
            gameState.playerAcceleration = 0.f;
        }
        else
        {
            gameState.playerAcceleration = ACCELERATION;
        }

        InitPlayer(gameState.player, gameState.playerTexture);
        gameState.player.speed = INITIAL_SPEED;

        for (int i = 0; i < gameState.applesCount; ++i)
        {
            InitApple(gameState.apples[i], gameState.appleTexture);
        }

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

            // Collision with apples
            for (int i = 0; i < gameState.applesCount; ++i)
            {
                if (HasPlayerCollisionWithApple(gameState.player, gameState.apples[i]))
                {
                    // Fixed: use MODE_APPLES_INFINITE
                    if (gameState.gameModeMask & MODE_APPLES_INFINITE)
                    {
                        // Infinite mode – just respawn the apple
                        InitApple(gameState.apples[i], gameState.appleTexture);
                    }
                    else
                    {
                        // Finite mode – decrease apples count
                        gameState.apples[i] = gameState.apples[gameState.applesCount - 1];
                        --gameState.applesCount;
                        --i;
                    }

                    ++gameState.numEatenApples;

                    if (gameState.numEatenApples > gameState.bestScore)
                    {
                        gameState.bestScore = gameState.numEatenApples;
                    }

                    gameState.eatAppleSound.play();

                    if (gameState.gameModeMask & MODE_SPEED_ACCEL_ON)
                    {
                        gameState.player.speed += gameState.playerAcceleration;
                    }

                    // If not infinite and no apples left – game over
                    if (!(gameState.gameModeMask & MODE_APPLES_INFINITE) &&
                        gameState.applesCount == 0)
                    {
                        gameState.isGameOver = true;
                        gameState.timeSinceGameOver = 0.f;

                        Record playerRecord;
                        playerRecord.name = "Player";
                        playerRecord.score = gameState.numEatenApples;
                        UpdateLeaderboardWithPlayer(gameState, playerRecord);

                        break;
                    }
                }
            }

            // Collision with obstacles
            for (int i = 0; i < gameState.currentObstaclesCount && !gameState.isGameOver; ++i)
            {
                if (HasPlayerCollisionWithObstacle(gameState.player, gameState.obstacles[i]))
                {
                    gameState.isGameOver = true;
                    gameState.timeSinceGameOver = 0.f;
                    gameState.hitSound.play();

                    Record playerRecord;
                    playerRecord.name = "Player";
                    playerRecord.score = gameState.numEatenApples;
                    UpdateLeaderboardWithPlayer(gameState, playerRecord);
                }
            }

            // Collision with screen borders
            if (!gameState.isGameOver &&
                HasPlayerCollisionWithScreenBorder(gameState.player))
            {
                gameState.isGameOver = true;
                gameState.timeSinceGameOver = 0.f;
                gameState.hitSound.play();

                Record playerRecord;
                playerRecord.name = "Player";
                playerRecord.score = gameState.numEatenApples;
                UpdateLeaderboardWithPlayer(gameState, playerRecord);
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

    void CleanupGame(GameState& gameState)
    {
        if (gameState.apples != nullptr)
        {
            delete[] gameState.apples;
            gameState.apples = nullptr;
        }
    }
}

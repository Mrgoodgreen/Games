#include "Game.h"
#include "Math.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cassert>

namespace ApplesGame
{
    // ---- Leaderboard helper data ----
    const char* NAMES_POOL[] = {
        "Alice", "Bob", "Carol", "Dave", "Eve",
        "Frank", "Grace", "Heidi", "Ivan", "Judy"
    };

    const int NAMES_POOL_SIZE = sizeof(NAMES_POOL) / sizeof(NAMES_POOL[0]);

    // Simple selection sort in descending order by score (without std::sort)
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
        count = std::max(5, std::min(count, 10)); // 5 to 10 records
        for (int i = 0; i < count; ++i)
        {
            Record r;
            r.name = NAMES_POOL[std::rand() % NAMES_POOL_SIZE];
            r.score = 20 + std::rand() % 120; // score 20-139
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

    // ---- Main game logic ----

    void InitGame(GameState& gameState)
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        // Load resources
        assert(gameState.playerTexture.loadFromFile(RESOURCES_PATH + "Pacman.png"));
        assert(gameState.appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png"));
        assert(gameState.obstacleTexture.loadFromFile(RESOURCES_PATH + "Obstacle.png"));
        assert(gameState.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

        // Load sounds
        assert(gameState.eatAppleBuffer.loadFromFile(RESOURCES_PATH + "Sounds/eat.wav"));
        assert(gameState.hitBuffer.loadFromFile(RESOURCES_PATH + "Sounds/hit.wav"));
        gameState.eatAppleSound.setBuffer(gameState.eatAppleBuffer);
        gameState.hitSound.setBuffer(gameState.hitBuffer);

        InitUI(gameState.uiState, gameState.font);

        // FIXED: Allocate memory for apples based on default count only - not MAX_APPLES
        if (gameState.apples != nullptr)
        {
            delete[] gameState.apples;
            gameState.apples = nullptr;
        }
        gameState.apples = new Apple[gameState.applesDefaultCount];

        RestartGame(gameState);
    }

    void RestartGame(GameState& gameState)
    {
        // Initialize leaderboard on first run only
        if (!gameState.isLeaderboardInitialized)
        {
            GenerateRandomLeaderboard(gameState, 7);
        }

        // FIXED: Restore default apple count on restart - separated from current count
        gameState.applesCount = gameState.applesDefaultCount;

        // Validate apple count
        if (gameState.applesCount <= 0 || gameState.applesCount > MAX_APPLES)
        {
            gameState.applesCount = 20;
            gameState.applesDefaultCount = 20;
        }

        gameState.currentObstaclesCount = BASE_NUM_OBSTACLES;

        // FIXED: Handle acceleration mode with explicit flags
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
            // Default: acceleration enabled if no explicit flag set
            gameState.playerAcceleration = ACCELERATION;
        }

        InitPlayer(gameState.player, gameState.playerTexture);
        gameState.player.speed = INITIAL_SPEED;

        // Initialize apples
        for (int i = 0; i < gameState.applesCount; ++i)
        {
            InitApple(gameState.apples[i], gameState.appleTexture);
        }

        // Initialize obstacles
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

        // Handle player direction input
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

            // Collision detection with apples
            for (int i = 0; i < gameState.applesCount; ++i)
            {
                if (HasPlayerCollisionWithApple(gameState.player, gameState.apples[i]))
                {
                    // FIXED: Handle finite mode - remove apple from field
                    if (gameState.gameModeMask & MODE_APPLES_FINITE)
                    {
                        gameState.apples[i] = gameState.apples[gameState.applesCount - 1];
                        --gameState.applesCount;
                        --i;
                    }
                    else
                    {
                        // Infinite mode - regenerate apple at new position
                        InitApple(gameState.apples[i], gameState.appleTexture);
                    }

                    gameState.numEatenApples++;
                    if (gameState.numEatenApples > gameState.bestScore)
                        gameState.bestScore = gameState.numEatenApples;

                    gameState.eatAppleSound.play();

                    // Apply acceleration if enabled
                    if (gameState.gameModeMask & MODE_SPEED_ACCEL_ON)
                    {
                        gameState.player.speed += gameState.playerAcceleration;
                    }

                    // FIXED: Check win condition in finite mode
                    if ((gameState.gameModeMask & MODE_APPLES_FINITE) && gameState.applesCount == 0)
                    {
                        gameState.isGameOver = true;
                        gameState.timeSinceGameOver = 0.f;
                        UpdateLeaderboardWithPlayer(gameState);
                        break;
                    }
                }
            }

            // Collision detection with obstacles
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

            // Collision detection with screen border
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

        // Draw apples
        for (int i = 0; i < gameState.applesCount; ++i)
        {
            DrawApple(gameState.apples[i], window);
        }

        // Draw obstacles
        for (int i = 0; i < gameState.currentObstaclesCount; ++i)
        {
            DrawObstacle(gameState.obstacles[i], window);
        }

        DrawUI(gameState.uiState, window);
    }

    // FIXED: Cleanup function to properly deallocate dynamic memory
    void CleanupGame(GameState& gameState)
    {
        if (gameState.apples != nullptr)
        {
            delete[] gameState.apples;
            gameState.apples = nullptr;
        }
    }

}
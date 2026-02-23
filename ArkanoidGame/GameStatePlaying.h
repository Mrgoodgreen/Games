#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameSettings.h"
#include <vector>

namespace ArkanoidGame
{
    // Game state for the playing screen. Resources are owned by this class (RAII).
    class GameStatePlayingData
    {
    private:
        // Resources
        sf::Font font;
        sf::SoundBuffer hitSoundBuffer;
        sf::SoundBuffer gameOverSoundBuffer;
        sf::Texture ballTexture;
        sf::Texture brickTexture;
        sf::Texture platformTexture;

        // Resource load flags
        bool fontLoaded = false;
        bool hitSoundLoaded = false;
        bool gameOverSoundLoaded = false;
        bool ballTextureLoaded = false;
        bool brickTextureLoaded = false;
        bool platformTextureLoaded = false;

        // Game objects (private: no direct external access)
        sf::RectangleShape platform;
        sf::CircleShape ball;
        std::vector<sf::RectangleShape> bricks;
        int score = 0;
        int lives = 3;

        // UI
        sf::Text scoreText;
        sf::Text livesText;
        sf::Text inputHintText;
        sf::RectangleShape background;

        // Debug overlay
        bool debugEnabled = false;
        sf::Text debugText;

        // Ball state
        bool isBallAttachedToPlatform = true;
        sf::Vector2f ballVelocity;
        float ballSpeed = INITIAL_BALL_SPEED;

        // Sounds
        sf::Sound hitSound;
        sf::Sound gameOverSound;

        // Helper: clamp value to range
        static float clamp(float v, float a, float b) { return (v < a) ? a : (v > b) ? b : v; }

    public:
        // Constructor sets up sizes/positions that depend on screen size
        GameStatePlayingData(float screenWidth, float screenHeight);

        // Read-only accessors for basic state
        int getScore() const { return score; }
        int getLives() const { return lives; }

        // Controlled platform API (no direct shape access)
        float getPlatformX() const { return platform.getPosition().x; }
        sf::FloatRect getPlatformBounds() const { return platform.getGlobalBounds(); }
        void setPlatformX(float x) { platform.setPosition(clamp(x, PLATFORM_WIDTH / 2.0f, SCREEN_WIDTH - PLATFORM_WIDTH / 2.0f), platform.getPosition().y); }
        void movePlatformBy(float dx) { platform.move(dx, 0.f); }

        // Controlled ball API
        bool isBallAttached() const { return isBallAttachedToPlatform; }
        void attachBallToPlatform() { isBallAttachedToPlatform = true; ball.setPosition(platform.getPosition().x, platform.getPosition().y - PLATFORM_HEIGHT / 2.0f - BALL_RADIUS); }
        void launchBallWithAngle(float degrees) {
            isBallAttachedToPlatform = false;
            float rad = degrees * 3.14159265f / 180.0f;
            ballVelocity = sf::Vector2f(cosf(rad), -sinf(rad)) * ballSpeed;
        }

        // Lifecycle and main operations (resources are members and freed automatically)
        void init();
        void handleWindowEvent(const sf::Event& event);
        void update(float timeDelta);
        void draw(sf::RenderWindow& window);

    private:
        // Internal helper: process ball collisions and game logic
        void handleBallCollisions(float timeDelta);
    };

    // Free-function wrappers used by Game.cpp to initialize/update/draw state
    void InitGameStatePlaying(GameStatePlayingData& data);
    void ShutdownGameStatePlaying(GameStatePlayingData& data);
    void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, const sf::Event& event);
    void UpdateGameStatePlaying(GameStatePlayingData& data, float timeDelta);
    void DrawGameStatePlaying(GameStatePlayingData& data, sf::RenderWindow& window);
}
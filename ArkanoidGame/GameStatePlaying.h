#pragma once
#include "IGameState.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameSettings.h"
#include "Brick.h"
#include <vector>

namespace ArkanoidGame
{
    // Playing state: main game logic with platform, ball, and bricks
    class PlayingState : public IGameState
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
        std::vector<Brick> bricks;
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
        PlayingState(float screenWidth, float screenHeight);
        ~PlayingState() = default;

        // IGameState interface implementation
        void onEnter() override;
        void onExit() override;
        void handleEvent(const sf::Event& event) override;
        void update(float timeDelta) override;
        void draw(sf::RenderWindow& window) override;
        const char* getStateName() const override { return "PlayingState"; }

        // State query methods
        int getScore() const { return score; }
        int getLives() const { return lives; }

    private:
        // Internal helpers
        void handleBallCollisions(float timeDelta);
        void loadResources();
        void applyTexturesOrFallback();
        void setupUI();
    };
}
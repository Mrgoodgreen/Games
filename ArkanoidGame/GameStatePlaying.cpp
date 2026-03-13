#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStateWin.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <cassert>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iostream>

namespace ArkanoidGame
{
    PlayingState::PlayingState(float screenWidth, float screenHeight)
    {
        std::cout << "[PlayingState] Constructor called" << std::endl;
        
        // Prepare platform geometry
        platform.setSize(sf::Vector2f(PLATFORM_WIDTH, PLATFORM_HEIGHT));
        platform.setOrigin(PLATFORM_WIDTH / 2.0f, PLATFORM_HEIGHT / 2.0f);
        platform.setPosition(screenWidth / 2.0f, screenHeight - PLATFORM_HEIGHT * 2.0f);

        // Prepare ball geometry
        ball.setRadius(BALL_RADIUS);
        ball.setOrigin(BALL_RADIUS, BALL_RADIUS);
        ball.setPosition(screenWidth / 2.0f, screenHeight - PLATFORM_HEIGHT * 3.0f);

        // Prepare bricks geometry
        for (int row = 0; row < BRICK_ROWS; ++row) {
            for (int col = 0; col < BRICK_COLUMNS; ++col) {
                Brick brick;
                brick.Initialize(BRICK_WIDTH - 2.f, BRICK_HEIGHT - 2.f,
                    col * BRICK_WIDTH + BRICK_WIDTH / 2.0f,
                    row * BRICK_HEIGHT + BRICK_HEIGHT * 3.0f);
                brick.SetOutline(sf::Color::Black, 1.0f);
                bricks.push_back(brick);
            }
        }

        // Prepare background - using lighter color for visibility
        background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        background.setPosition(0.f, 0.f);
        background.setFillColor(sf::Color(30, 30, 80));  // Slightly lighter dark blue
        
        std::cout << "[PlayingState] Background size: " << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << std::endl;
        std::cout << "[PlayingState] Bricks created: " << bricks.size() << std::endl;

        // Initial ball state
        isBallAttachedToPlatform = true;
        ballVelocity = sf::Vector2f(0.f, -ballSpeed);
        
        std::cout << "[PlayingState] Constructor complete - geometry initialized" << std::endl;
    }

    void PlayingState::onEnter()
    {
        std::cout << "[PlayingState] onEnter() called" << std::endl;
        
        // Load resources when state becomes active
        loadResources();
        applyTexturesOrFallback();
        setupUI();
        
        std::cout << "[PlayingState] onEnter() complete" << std::endl;
    }

    void PlayingState::onExit()
    {
        std::cout << "[PlayingState] onExit() called" << std::endl;
        // Resources cleaned up by destructor (RAII)
    }

    void PlayingState::loadResources()
    {
        std::cout << "[PlayingState] Loading resources..." << std::endl;
        
        // Load font and sounds with fallback flags
        fontLoaded = font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf");
        std::cout << "[PlayingState] Font loaded: " << (fontLoaded ? "YES" : "NO") << std::endl;
        
        hitSoundLoaded = hitSoundBuffer.loadFromFile(SOUNDS_PATH + "Hit.wav");
        gameOverSoundLoaded = gameOverSoundBuffer.loadFromFile(SOUNDS_PATH + "GameOver.wav");

        // Load textures with fallback flags
        ballTextureLoaded = ballTexture.loadFromFile(TEXTURES_PATH + "Ball.png");
        std::cout << "[PlayingState] Ball texture loaded: " << (ballTextureLoaded ? "YES" : "NO") << std::endl;
        
        brickTextureLoaded = brickTexture.loadFromFile(TEXTURES_PATH + "Brick.png");
        std::cout << "[PlayingState] Brick texture loaded: " << (brickTextureLoaded ? "YES" : "NO") << std::endl;
        
        platformTextureLoaded = platformTexture.loadFromFile(TEXTURES_PATH + "Platform.png");
        std::cout << "[PlayingState] Platform texture loaded: " << (platformTextureLoaded ? "YES" : "NO") << std::endl;

        // Set up sounds if loaded
        if (hitSoundLoaded) hitSound.setBuffer(hitSoundBuffer);
        if (gameOverSoundLoaded) gameOverSound.setBuffer(gameOverSoundBuffer);
    }

    void PlayingState::applyTexturesOrFallback()
    {
        // Apply platform texture or fallback color
        if (platformTextureLoaded) {
            platform.setTexture(&platformTexture);
        }
        else {
            platform.setFillColor(sf::Color::Blue);
        }

        // Apply ball texture or fallback color
        if (ballTextureLoaded) {
            ball.setTexture(&ballTexture);
        }
        else {
            ball.setFillColor(sf::Color::Red);
        }

        // Apply brick textures or colored fallback
        if (brickTextureLoaded) {
            for (auto& brick : bricks) {
                brick.SetFillColor(sf::Color::White);
                brick.SetTexture(&brickTexture);
            }
        }
        else {
            for (size_t i = 0; i < bricks.size(); ++i) {
                int row = static_cast<int>(i) / BRICK_COLUMNS;
                bricks[i].SetFillColor(sf::Color(200 - row * 30, 100 + row * 10, 100));
            }
        }
    }

    void PlayingState::setupUI()
    {
        // Only set up UI if font loaded
        if (fontLoaded) {
            scoreText.setFont(font);
            scoreText.setCharacterSize(24);
            scoreText.setFillColor(sf::Color::Yellow);

            livesText.setFont(font);
            livesText.setCharacterSize(24);
            livesText.setFillColor(sf::Color::Yellow);

            inputHintText.setFont(font);
            inputHintText.setCharacterSize(18);
            inputHintText.setFillColor(sf::Color::White);
            inputHintText.setString("Use arrow keys or mouse to move platform, SPACE to release ball, ESC to pause");
            inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 1.f }));

            debugText.setFont(font);
            debugText.setCharacterSize(14);
            debugText.setFillColor(sf::Color::White);
        }
    }

    void PlayingState::handleEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space && isBallAttachedToPlatform)
            {
                // Launch ball with a small random angle
                float angleDeg = static_cast<float>((rand() % 60) - 30);
                float rad = angleDeg * 3.14159265f / 180.0f;
                isBallAttachedToPlatform = false;
                ballVelocity = sf::Vector2f(cosf(rad), -sinf(rad)) * ballSpeed;
            }
            else if (event.key.code == sf::Keyboard::F3)
            {
                debugEnabled = !debugEnabled;
            }
        }
    }

    void PlayingState::update(float timeDelta)
    {
        // Keyboard input - exclusive handling
        bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);

        if (leftPressed && !rightPressed) {
            platform.move(-PLATFORM_SPEED * timeDelta, 0.f);
        }
        else if (rightPressed && !leftPressed) {
            platform.move(PLATFORM_SPEED * timeDelta, 0.f);
        }
        else {
            // Follow mouse X position (window-relative) when no keys pressed
            sf::Vector2i mousePos = sf::Mouse::getPosition(Application::Instance().GetWindow());
            float mouseX = static_cast<float>(mousePos.x);
            float platformX = clamp(mouseX, PLATFORM_WIDTH / 2.0f, SCREEN_WIDTH - PLATFORM_WIDTH / 2.0f);
            platform.setPosition(platformX, platform.getPosition().y);
        }

        // Clamp platform to screen bounds
        float platformX = clamp(platform.getPosition().x, PLATFORM_WIDTH / 2.0f, SCREEN_WIDTH - PLATFORM_WIDTH / 2.0f);
        platform.setPosition(platformX, platform.getPosition().y);

        // Ball update
        if (!isBallAttachedToPlatform) {
            ball.move(ballVelocity * timeDelta);
            handleBallCollisions(timeDelta);
        }
        else {
            // Keep ball attached to platform
            ball.setPosition(platform.getPosition().x,
                platform.getPosition().y - PLATFORM_HEIGHT / 2.0f - BALL_RADIUS);
        }

        // Update UI
        if (fontLoaded) {
            scoreText.setString("Score: " + std::to_string(score));
            livesText.setString("Lives: " + std::to_string(lives));

            if (debugEnabled) {
                std::ostringstream s;
                s << "PlatformX=" << platform.getPosition().x << " Ball=(" << ball.getPosition().x << "," << ball.getPosition().y << ")";
                debugText.setString(s.str());
            }
        }
    }

    void PlayingState::handleBallCollisions(float /*timeDelta*/)
    {
        float ballLeft = ball.getPosition().x - BALL_RADIUS;
        float ballRight = ball.getPosition().x + BALL_RADIUS;
        float ballTop = ball.getPosition().y - BALL_RADIUS;
        float ballBottom = ball.getPosition().y + BALL_RADIUS;

        // Left/Right walls
        if (ballLeft < 0.f || ballRight > SCREEN_WIDTH) {
            ballVelocity.x = -ballVelocity.x;
            if (ballLeft < 0.f) ball.setPosition(BALL_RADIUS, ball.getPosition().y);
            else if (ballRight > SCREEN_WIDTH) ball.setPosition(SCREEN_WIDTH - BALL_RADIUS, ball.getPosition().y);
        }

        // Top wall
        if (ballTop < 0.f) {
            ballVelocity.y = -ballVelocity.y;
            ball.setPosition(ball.getPosition().x, BALL_RADIUS);
        }

        // Bottom: life lost
        if (ballBottom > SCREEN_HEIGHT) {
            lives--;

            if (lives <= 0) {
                if (gameOverSoundLoaded) gameOverSound.play();
                std::cout << "[PlayingState] Game Over! Score: " << score << std::endl;
                // Push game over state
                Game& game = Application::Instance().GetGame();
                auto& records = game.getRecordsTable();
                records[PLAYER_NAME] = std::max(records[PLAYER_NAME], score);
                game.pushState(std::make_unique<GameOverState>());
            }
            else {
                isBallAttachedToPlatform = true;
                ball.setPosition(platform.getPosition().x,
                    platform.getPosition().y - PLATFORM_HEIGHT / 2.0f - BALL_RADIUS);
            }
            return;
        }

        // Platform bounce
        if (ball.getGlobalBounds().intersects(platform.getGlobalBounds())) {
            float platformCenter = platform.getPosition().x;
            float ballCenter = ball.getPosition().x;
            float relativeIntersectX = (ballCenter - platformCenter) / (PLATFORM_WIDTH / 2.0f);

            float bounceAngle = relativeIntersectX * 5.0f * 3.14159265f / 12.0f;

            ballVelocity.x = ballSpeed * cosf(bounceAngle);
            ballVelocity.y = -ballSpeed * sinf(bounceAngle);

            if (ballVelocity.y > 0.f) ballVelocity.y = -ballVelocity.y;

            if (hitSoundLoaded) hitSound.play();

            ball.setPosition(ball.getPosition().x, platform.getPosition().y - PLATFORM_HEIGHT / 2.0f - BALL_RADIUS);
        }

        // Bricks collisions
        for (auto it = bricks.begin(); it != bricks.end();) {
            if (ball.getGlobalBounds().intersects(it->GetBounds())) {
                float ballCenterX = ball.getPosition().x;
                float ballCenterY = ball.getPosition().y;
                float brickCenterX = it->GetPosition().x;
                float brickCenterY = it->GetPosition().y;

                float dx = std::abs(ballCenterX - brickCenterX) - (BRICK_WIDTH / 2.0f + BALL_RADIUS);
                float dy = std::abs(ballCenterY - brickCenterY) - (BRICK_HEIGHT / 2.0f + BALL_RADIUS);

                if (dx > dy) {
                    ballVelocity.x = -ballVelocity.x;
                }
                else {
                    ballVelocity.y = -ballVelocity.y;
                }

                it = bricks.erase(it);
                score += 10;
                if (hitSoundLoaded) hitSound.play();
            }
            else {
                ++it;
            }
        }

        // Win condition
        if (bricks.empty()) {
            std::cout << "[PlayingState] All bricks destroyed! Score: " << score << std::endl;
            Game& game = Application::Instance().GetGame();
            auto& records = game.getRecordsTable();
            records[PLAYER_NAME] = std::max(records[PLAYER_NAME], score);
            game.pushState(std::make_unique<WinState>());
        }
    }

    void PlayingState::draw(sf::RenderWindow& window)
    {
        window.draw(background);
        for (const auto& brick : bricks) brick.Draw(window);
        window.draw(platform);
        window.draw(ball);

        if (fontLoaded) {
            scoreText.setOrigin(GetTextOrigin(scoreText, { 0.f, 0.f }));
            scoreText.setPosition(10.f, 10.f);
            window.draw(scoreText);

            livesText.setOrigin(GetTextOrigin(livesText, { 1.f, 0.f }));
            livesText.setPosition(SCREEN_WIDTH - 10.f, 10.f);
            window.draw(livesText);

            sf::Vector2f viewSize = window.getView().getSize();
            inputHintText.setPosition(viewSize.x - 10.f, viewSize.y - 10.f);
            window.draw(inputHintText);

            if (debugEnabled) {
                debugText.setPosition(10.f, SCREEN_HEIGHT - 30.f);
                window.draw(debugText);
            }
        }
    }
}
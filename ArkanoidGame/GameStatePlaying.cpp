#include "GameStatePlaying.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <cassert>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <algorithm>

namespace ArkanoidGame
{
    // Constructor sets up geometry and loads resources (RAII).
    GameStatePlayingData::GameStatePlayingData(float screenWidth, float screenHeight)
    {
        // Prepare platform and ball geometry
        platform.setSize(sf::Vector2f(PLATFORM_WIDTH, PLATFORM_HEIGHT));
        platform.setOrigin(PLATFORM_WIDTH / 2.0f, PLATFORM_HEIGHT / 2.0f);
        platform.setPosition(screenWidth / 2.0f, screenHeight - PLATFORM_HEIGHT * 2.0f);

        ball.setRadius(BALL_RADIUS);
        ball.setOrigin(BALL_RADIUS, BALL_RADIUS);
        ball.setPosition(screenWidth / 2.0f, screenHeight - PLATFORM_HEIGHT * 3.0f);

        // Prepare bricks geometry (textures will be applied after loading)
        for (int row = 0; row < BRICK_ROWS; ++row) {
            for (int col = 0; col < BRICK_COLUMNS; ++col) {
                sf::RectangleShape brick(sf::Vector2f(BRICK_WIDTH - 2.f, BRICK_HEIGHT - 2.f));
                brick.setOutlineColor(sf::Color::Black);
                brick.setOutlineThickness(1.0f);
                brick.setOrigin(BRICK_WIDTH / 2.0f, BRICK_HEIGHT / 2.0f);
                brick.setPosition(col * BRICK_WIDTH + BRICK_WIDTH / 2.0f,
                    row * BRICK_HEIGHT + BRICK_HEIGHT * 3.0f);
                bricks.push_back(brick);
            }
        }

        // Load resources (fall back to colors if textures/fonts missing)
        fontLoaded = font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf");
        hitSoundLoaded = hitSoundBuffer.loadFromFile(SOUNDS_PATH + "Hit.wav");
        gameOverSoundLoaded = gameOverSoundBuffer.loadFromFile(SOUNDS_PATH + "GameOver.wav");

        ballTextureLoaded = ballTexture.loadFromFile(TEXTURES_PATH + "Ball.png");
        brickTextureLoaded = brickTexture.loadFromFile(TEXTURES_PATH + "Brick.png");
        platformTextureLoaded = platformTexture.loadFromFile(TEXTURES_PATH + "Platform.png");

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
                brick.setFillColor(sf::Color::White);
                brick.setTexture(&brickTexture);
            }
        }
        else {
            for (size_t i = 0; i < bricks.size(); ++i) {
                int row = static_cast<int>(i) / BRICK_COLUMNS;
                bricks[i].setFillColor(sf::Color(200 - row * 30, 100 + row * 10, 100));
            }
        }

        // Background
        background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        background.setPosition(0.f, 0.f);
        background.setFillColor(sf::Color(0, 0, 50));

        // UI (only set up if font loaded)
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

        // Sounds
        if (hitSoundLoaded) hitSound.setBuffer(hitSoundBuffer);
        if (gameOverSoundLoaded) gameOverSound.setBuffer(gameOverSoundBuffer);

        // Initial ball state
        isBallAttachedToPlatform = true;
        ballVelocity = sf::Vector2f(0.f, -ballSpeed);
    }

    void GameStatePlayingData::init()
    {
        // Initialization performed in constructor (strict RAII)
    }

    void GameStatePlayingData::handleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                PushGameState(Application::Instance().GetGame(), GameStateType::PauseMenu, false);
            }
            else if (event.key.code == sf::Keyboard::Space && isBallAttachedToPlatform)
            {
                float angleDeg = static_cast<float>((rand() % 60) - 30);
                launchBallWithAngle(angleDeg);
            }
            else if (event.key.code == sf::Keyboard::F3)
            {
                debugEnabled = !debugEnabled;
            }
        }
    }

    void GameStatePlayingData::update(float timeDelta)
    {
        // Keyboard input - exclusive handling
        bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);

        if (leftPressed && !rightPressed) {
            movePlatformBy(-PLATFORM_SPEED * timeDelta);
        }
        else if (rightPressed && !leftPressed) {
            movePlatformBy(PLATFORM_SPEED * timeDelta);
        }
        else {
            // Follow mouse X position (window-relative) when no keys pressed
            sf::Vector2i mousePos = sf::Mouse::getPosition(Application::Instance().GetWindow());
            float mouseX = static_cast<float>(mousePos.x);
            setPlatformX(clamp(mouseX, PLATFORM_WIDTH / 2.0f, SCREEN_WIDTH - PLATFORM_WIDTH / 2.0f));
        }

        // Clamp platform to screen bounds
        setPlatformX(clamp(getPlatformX(), PLATFORM_WIDTH / 2.0f, SCREEN_WIDTH - PLATFORM_WIDTH / 2.0f));

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
        }

        // Update debug overlay text
        if (debugEnabled && fontLoaded) {
            std::ostringstream s;
            s << "PlatformX=" << getPlatformX() << " Ball=(" << ball.getPosition().x << "," << ball.getPosition().y << ")";
            debugText.setString(s.str());
        }
    }

    void GameStatePlayingData::handleBallCollisions(float /*timeDelta*/)
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
            if (!IsEnableOptions(Application::Instance().GetGame(), GameOptions::InfiniteLives)) {
                lives--;
            }

            if (lives <= 0) {
                if (gameOverSoundLoaded) gameOverSound.play();
                Game& game = Application::Instance().GetGame();
                auto& records = game.getRecordsTable();
                records[PLAYER_NAME] = std::max(records[PLAYER_NAME], score);
                PushGameState(game, GameStateType::GameOver, false);
            }
            else {
                attachBallToPlatform();
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
            if (ball.getGlobalBounds().intersects(it->getGlobalBounds())) {
                float ballCenterX = ball.getPosition().x;
                float ballCenterY = ball.getPosition().y;
                float brickCenterX = it->getPosition().x;
                float brickCenterY = it->getPosition().y;

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
            Game& game = Application::Instance().GetGame();
            auto& records = game.getRecordsTable();
            records[PLAYER_NAME] = std::max(records[PLAYER_NAME], score);
            PushGameState(game, GameStateType::GameOver, false);
        }
    }

    void GameStatePlayingData::draw(sf::RenderWindow& window)
    {
        window.draw(background);
        for (const auto& brick : bricks) window.draw(brick);
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

    // Free function wrappers
    void InitGameStatePlaying(GameStatePlayingData& data) { /* constructor did init() */ }
    void ShutdownGameStatePlaying(GameStatePlayingData& data) { /* destructor will cleanup */ }
    void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, const sf::Event& event) { data.handleWindowEvent(event); }
    void UpdateGameStatePlaying(GameStatePlayingData& data, float timeDelta) { data.update(timeDelta); }
    void DrawGameStatePlaying(GameStatePlayingData& data, sf::RenderWindow& window) { data.draw(window); }
}
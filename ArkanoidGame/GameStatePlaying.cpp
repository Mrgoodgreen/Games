#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStateWin.h"
#include "Collision.h"
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

        platform.setSize(sf::Vector2f(PLATFORM_WIDTH, PLATFORM_HEIGHT));
        platform.setOrigin(PLATFORM_WIDTH / 2.0f, PLATFORM_HEIGHT / 2.0f);
        platform.setPosition(screenWidth / 2.0f, screenHeight - PLATFORM_HEIGHT * 2.0f);

        ball.setRadius(BALL_RADIUS);
        ball.setOrigin(BALL_RADIUS, BALL_RADIUS);
        ball.setPosition(screenWidth / 2.0f, screenHeight - PLATFORM_HEIGHT * 3.0f);

        bricks.reserve(BRICK_ROWS * BRICK_COLUMNS);
        for (int row = 0; row < static_cast<int>(BRICK_ROWS); ++row)
        {
            for (int col = 0; col < static_cast<int>(BRICK_COLUMNS); ++col)
            {
                std::unique_ptr<Brick> brick;

                if ((row + col) % 7 == 0)
                {
                    brick = std::make_unique<GlassBrick>();
                }
                else if (row == 0 || (row + col) % 4 == 0)
                {
                    brick = std::make_unique<StrongBrick>();
                }
                else
                {
                    brick = std::make_unique<Brick>();
                }

                brick->Initialize(
                    BRICK_WIDTH - 2.f,
                    BRICK_HEIGHT - 2.f,
                    col * BRICK_WIDTH + BRICK_WIDTH / 2.0f,
                    row * BRICK_HEIGHT + BRICK_HEIGHT * 3.0f);
                brick->SetOutline(sf::Color::Black, 1.0f);
                bricks.push_back(std::move(brick));
            }
        }

        background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        background.setPosition(0.f, 0.f);
        background.setFillColor(sf::Color(30, 30, 80));

        std::cout << "[PlayingState] Background size: " << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << std::endl;
        std::cout << "[PlayingState] Bricks created: " << bricks.size() << std::endl;

        isBallAttachedToPlatform = true;
        ballVelocity = sf::Vector2f(0.f, -ballSpeed);

        std::cout << "[PlayingState] Constructor complete - geometry initialized" << std::endl;
    }

    void PlayingState::onEnter()
    {
        std::cout << "[PlayingState] onEnter() called" << std::endl;

        scoreSystem.AddObserver(this);
        scoreSystem.Reset();
        lives = 3;
        scoreMultiplier = 1;

        loadResources();
        applyTexturesOrFallback();
        setupUI();
        createLifeMemento();

        std::cout << "[PlayingState] onEnter() complete" << std::endl;
    }

    void PlayingState::onExit()
    {
        clearBonuses();
        scoreSystem.RemoveObserver(this);
        std::cout << "[PlayingState] onExit() called" << std::endl;
    }

    void PlayingState::OnScoreChanged(int newScore)
    {
        score = newScore;
    }

    void PlayingState::loadResources()
    {
        std::cout << "[PlayingState] Loading resources..." << std::endl;

        fontLoaded = font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf");
        std::cout << "[PlayingState] Font loaded: " << (fontLoaded ? "YES" : "NO") << std::endl;

        hitSoundLoaded = hitSoundBuffer.loadFromFile(SOUNDS_PATH + "Hit.wav");
        gameOverSoundLoaded = gameOverSoundBuffer.loadFromFile(SOUNDS_PATH + "GameOver.wav");

        ballTextureLoaded = ballTexture.loadFromFile(TEXTURES_PATH + "Ball.png");
        std::cout << "[PlayingState] Ball texture loaded: " << (ballTextureLoaded ? "YES" : "NO") << std::endl;

        brickTextureLoaded = brickTexture.loadFromFile(TEXTURES_PATH + "Brick.png");
        std::cout << "[PlayingState] Brick texture loaded: " << (brickTextureLoaded ? "YES" : "NO") << std::endl;

        platformTextureLoaded = platformTexture.loadFromFile(TEXTURES_PATH + "Platform.png");
        std::cout << "[PlayingState] Platform texture loaded: " << (platformTextureLoaded ? "YES" : "NO") << std::endl;

        if (hitSoundLoaded) hitSound.setBuffer(hitSoundBuffer);
        if (gameOverSoundLoaded) gameOverSound.setBuffer(gameOverSoundBuffer);
    }

    void PlayingState::applyTexturesOrFallback()
    {
        if (platformTextureLoaded)
        {
            platform.setTexture(&platformTexture);
        }
        else
        {
            platform.setFillColor(sf::Color::Blue);
        }

        if (ballTextureLoaded)
        {
            ball.setTexture(&ballTexture);
        }
        else
        {
            ball.setFillColor(sf::Color::Red);
        }

        if (brickTextureLoaded)
        {
            for (auto& brick : bricks)
            {
                brick->SetTexture(&brickTexture);
                brick->SetFillColor(sf::Color::White);
            }
        }
        else
        {
            for (size_t i = 0; i < bricks.size(); ++i)
            {
                int row = static_cast<int>(i) / BRICK_COLUMNS;
                bricks[i]->SetFillColor(sf::Color(200 - row * 30, 100 + row * 10, 100));
            }
        }
    }

    void PlayingState::setupUI()
    {
        if (fontLoaded)
        {
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
                float angleDeg = static_cast<float>((rand() % 60) - 30);
                float rad = angleDeg * 3.14159265f / 180.0f;
                isBallAttachedToPlatform = false;
                ballVelocity = sf::Vector2f(cosf(rad), -sinf(rad)) * ballSpeed;
                createLifeMemento();
            }
            else if (event.key.code == sf::Keyboard::F3)
            {
                debugEnabled = !debugEnabled;
            }
        }
    }

    void PlayingState::update(float timeDelta)
    {
        bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);

        if (leftPressed && !rightPressed)
        {
            platform.move(-PLATFORM_SPEED * timeDelta, 0.f);
        }
        else if (rightPressed && !leftPressed)
        {
            platform.move(PLATFORM_SPEED * timeDelta, 0.f);
        }
        else
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(Application::Instance().GetWindow());
            float mouseX = static_cast<float>(mousePos.x);
            float platformX = clamp(mouseX, platform.getSize().x / 2.0f, SCREEN_WIDTH - platform.getSize().x / 2.0f);
            platform.setPosition(platformX, platform.getPosition().y);
        }

        float platformX = clamp(platform.getPosition().x, platform.getSize().x / 2.0f, SCREEN_WIDTH - platform.getSize().x / 2.0f);
        platform.setPosition(platformX, platform.getPosition().y);

        if (!isBallAttachedToPlatform)
        {
            ball.move(ballVelocity * timeDelta);
            handleBallCollisions(timeDelta);
        }
        else
        {
            ball.setPosition(platform.getPosition().x,
                platform.getPosition().y - platform.getSize().y / 2.0f - BALL_RADIUS);
        }

        updateBonuses(timeDelta);

        if (fontLoaded)
        {
            scoreText.setString("Score: " + std::to_string(score));
            livesText.setString("Lives: " + std::to_string(lives));

            if (debugEnabled)
            {
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

        if (ballLeft < 0.f || ballRight > SCREEN_WIDTH)
        {
            ballVelocity.x = -ballVelocity.x;
            if (ballLeft < 0.f) ball.setPosition(BALL_RADIUS, ball.getPosition().y);
            else if (ballRight > SCREEN_WIDTH) ball.setPosition(SCREEN_WIDTH - BALL_RADIUS, ball.getPosition().y);
        }

        if (ballTop < 0.f)
        {
            ballVelocity.y = -ballVelocity.y;
            ball.setPosition(ball.getPosition().x, BALL_RADIUS);
        }

        if (ballBottom > SCREEN_HEIGHT)
        {
            lives--;

            if (lives <= 0)
            {
                if (gameOverSoundLoaded) gameOverSound.play();
                std::cout << "[PlayingState] Game Over! Score: " << score << std::endl;
                Game& game = Application::Instance().GetGame();
                auto& records = game.getRecordsTable();
                records[PLAYER_NAME] = std::max(records[PLAYER_NAME], score);
                game.setLastScore(score);
                game.pushState(std::make_unique<GameOverState>());
            }
            else
            {
                restoreFromLifeMemento();
            }
            return;
        }

        if (ball.getGlobalBounds().intersects(platform.getGlobalBounds()))
        {
            float platformCenter = platform.getPosition().x;
            float ballCenter = ball.getPosition().x;
            float relativeIntersectX = (ballCenter - platformCenter) / (platform.getSize().x / 2.0f);

            float bounceAngle = relativeIntersectX * 5.0f * 3.14159265f / 12.0f;

            ballVelocity.x = ballSpeed * cosf(bounceAngle);
            ballVelocity.y = -ballSpeed * sinf(bounceAngle);

            if (ballVelocity.y > 0.f) ballVelocity.y = -ballVelocity.y;

            if (hitSoundLoaded) hitSound.play();

            ball.setPosition(ball.getPosition().x, platform.getPosition().y - platform.getSize().y / 2.0f - BALL_RADIUS);
        }

        for (auto it = bricks.begin(); it != bricks.end();)
        {
            Brick& brick = **it;
            if (Collision::Intersects(ball.getGlobalBounds(), brick))
            {
                if (brick.ShouldReflectBall())
                {
                    BounceAxis axis = Collision::GetBounceAxis(ball.getPosition(), BALL_RADIUS, brick.GetBounds());
                    Collision::ReflectVelocity(ballVelocity, axis);
                }

                const sf::Vector2f brickPos = brick.GetPosition();
                const bool destroyed = brick.OnHit();
                if (hitSoundLoaded) hitSound.play();

                if (destroyed)
                {
                    scoreSystem.AddPoints(brick.GetScoreValue() * scoreMultiplier);
                    std::unique_ptr<Bonus> bonus = BonusFactory::TryCreate(brickPos);
                    if (bonus)
                    {
                        fallingBonuses.push_back(std::move(bonus));
                    }

                    it = bricks.erase(it);
                }
                else
                {
                    ++it;
                }

                break;
            }
            else
            {
                ++it;
            }
        }

        if (bricks.empty())
        {
            std::cout << "[PlayingState] All bricks destroyed! Score: " << score << std::endl;
            Game& game = Application::Instance().GetGame();
            auto& records = game.getRecordsTable();
            records[PLAYER_NAME] = std::max(records[PLAYER_NAME], score);
            game.setLastScore(score);
            game.pushState(std::make_unique<WinState>());
        }
    }

    void PlayingState::draw(sf::RenderWindow& window)
    {
        window.draw(background);
        for (const auto& brick : bricks) brick->Draw(window);
        for (const auto& bonus : fallingBonuses) bonus->Draw(window);
        window.draw(platform);
        window.draw(ball);

        if (fontLoaded)
        {
            scoreText.setOrigin(GetTextOrigin(scoreText, { 0.f, 0.f }));
            scoreText.setPosition(10.f, 10.f);
            window.draw(scoreText);

            livesText.setOrigin(GetTextOrigin(livesText, { 1.f, 0.f }));
            livesText.setPosition(SCREEN_WIDTH - 10.f, 10.f);
            window.draw(livesText);

            sf::Vector2f viewSize = window.getView().getSize();
            inputHintText.setPosition(viewSize.x - 10.f, viewSize.y - 10.f);
            window.draw(inputHintText);

            if (debugEnabled)
            {
                debugText.setPosition(10.f, SCREEN_HEIGHT - 30.f);
                window.draw(debugText);
            }
        }
    }

    std::unique_ptr<Brick> PlayingState::createBrickByType(BrickType type) const
    {
        if (type == BrickType::Strong)
            return std::make_unique<StrongBrick>();
        if (type == BrickType::Glass)
            return std::make_unique<GlassBrick>();

        return std::make_unique<Brick>();
    }

    void PlayingState::createLifeMemento()
    {
        m_LastLifeMemento = std::make_unique<PlayingMemento>();
        m_LastLifeMemento->score = score;
        m_LastLifeMemento->bricks.reserve(bricks.size());

        for (const auto& brick : bricks)
        {
            BrickSnapshot snapshot;
            snapshot.type = brick->GetType();
            snapshot.position = brick->GetPosition();
            snapshot.durability = brick->GetDurability();
            m_LastLifeMemento->bricks.push_back(snapshot);
        }
    }

    void PlayingState::restoreFromLifeMemento()
    {
        clearBonuses();

        if (!m_LastLifeMemento)
        {
            isBallAttachedToPlatform = true;
            ballSpeed = INITIAL_BALL_SPEED;
            ballVelocity = sf::Vector2f(0.f, -ballSpeed);
            ball.setPosition(platform.getPosition().x,
                platform.getPosition().y - platform.getSize().y / 2.0f - BALL_RADIUS);
            return;
        }

        bricks.clear();
        bricks.reserve(m_LastLifeMemento->bricks.size());

        for (const BrickSnapshot& snapshot : m_LastLifeMemento->bricks)
        {
            std::unique_ptr<Brick> brick = createBrickByType(snapshot.type);
            brick->Initialize(BRICK_WIDTH - 2.f, BRICK_HEIGHT - 2.f, snapshot.position.x, snapshot.position.y);
            brick->SetDurability(snapshot.durability);
            brick->SetOutline(sf::Color::Black, 1.0f);
            bricks.push_back(std::move(brick));
        }

        applyTexturesOrFallback();

        scoreSystem.Reset();
        scoreSystem.AddPoints(m_LastLifeMemento->score);

        platform.setSize(sf::Vector2f(PLATFORM_WIDTH, PLATFORM_HEIGHT));
        platform.setOrigin(PLATFORM_WIDTH / 2.f, PLATFORM_HEIGHT / 2.f);

        scoreMultiplier = 1;
        ballSpeed = INITIAL_BALL_SPEED;
        ballVelocity = sf::Vector2f(0.f, -ballSpeed);
        isBallAttachedToPlatform = true;
        ball.setPosition(platform.getPosition().x,
            platform.getPosition().y - platform.getSize().y / 2.0f - BALL_RADIUS);
    }

    BonusRuntimeContext PlayingState::buildBonusContext()
    {
        return BonusRuntimeContext{ platform, ballVelocity, ballSpeed, scoreMultiplier };
    }

    void PlayingState::updateBonuses(float timeDelta)
    {
        for (auto it = fallingBonuses.begin(); it != fallingBonuses.end();)
        {
            (*it)->Update(timeDelta);

            if ((*it)->GetBounds().intersects(platform.getGlobalBounds()))
            {
                BonusRuntimeContext ctx = buildBonusContext();
                (*it)->Apply(ctx);

                ActiveBonus active;
                active.remainingTime = (*it)->GetDuration();
                active.bonus = std::move(*it);
                activeBonuses.push_back(std::move(active));

                it = fallingBonuses.erase(it);
                continue;
            }

            if ((*it)->GetPosition().y > SCREEN_HEIGHT + 20.f)
            {
                it = fallingBonuses.erase(it);
                continue;
            }

            ++it;
        }

        for (auto it = activeBonuses.begin(); it != activeBonuses.end();)
        {
            it->remainingTime -= timeDelta;
            if (it->remainingTime <= 0.f)
            {
                BonusRuntimeContext ctx = buildBonusContext();
                it->bonus->Revert(ctx);
                it = activeBonuses.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void PlayingState::clearBonuses()
    {
        BonusRuntimeContext ctx = buildBonusContext();
        for (auto& active : activeBonuses)
        {
            active.bonus->Revert(ctx);
        }

        activeBonuses.clear();
        fallingBonuses.clear();
        scoreMultiplier = 1;
    }
}
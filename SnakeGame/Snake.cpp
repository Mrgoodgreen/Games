#include "Snake.h"
#include "GameConfig.h"
#include <cassert>

Snake::Snake()
    : m_CurrentDirection(Utils::Direction::Right)
    , m_NextDirection(Utils::Direction::Right)
    , m_MoveTimer(0.0f)
    , m_GrowthPending(0)
{
}

Snake::~Snake()
{
}

void Snake::Initialize(sf::Vector2i startPosition, int length)
{
    m_Body.clear();
    m_CurrentDirection = Utils::Direction::Right;
    m_NextDirection = Utils::Direction::Right;
    m_MoveTimer = 0.0f;
    m_GrowthPending = 0;

    for (int i = 0; i < length; ++i)
    {
        m_Body.push_back(sf::Vector2i(startPosition.x - i, startPosition.y));
    }
}

void Snake::Update(float deltaTime, float speed)
{
    if (speed <= 0.0f)
    {
        return;
    }

    m_MoveTimer += deltaTime;
    float moveInterval = 1.0f / speed;

    if (m_MoveTimer >= moveInterval)
    {
        m_MoveTimer -= moveInterval;
        m_CurrentDirection = m_NextDirection;

        sf::Vector2i dirVector = Utils::GetDirectionVector(m_CurrentDirection);
        sf::Vector2i newHeadPos = m_Body[0] + dirVector;

        m_Body.insert(m_Body.begin(), newHeadPos);

        if (m_GrowthPending > 0)
        {
            m_GrowthPending--;
        }
        else
        {
            m_Body.pop_back();
        }
    }
}

void Snake::Draw(sf::RenderWindow& window)
{
    for (size_t i = 0; i < m_Body.size(); ++i)
    {
        sf::Vector2i gridPos = m_Body[i];
        sf::Vector2f screenPos(
            GameConfig::FieldOffsetX + gridPos.x * GameConfig::GridSize,
            GameConfig::FieldOffsetY + gridPos.y * GameConfig::GridSize
        );

        if (i == 0)
        {
            m_HeadSprite.setPosition(screenPos);
            
            if (m_CurrentDirection == Utils::Direction::Up)
            {
                m_HeadSprite.setRotation(270.0f);
            }
            else if (m_CurrentDirection == Utils::Direction::Down)
            {
                m_HeadSprite.setRotation(90.0f);
            }
            else if (m_CurrentDirection == Utils::Direction::Left)
            {
                m_HeadSprite.setRotation(180.0f);
            }
            else
            {
                m_HeadSprite.setRotation(0.0f);
            }
            
            window.draw(m_HeadSprite);
        }
        else
        {
            m_BodySprite.setPosition(screenPos);
            window.draw(m_BodySprite);
        }
    }
}

void Snake::Grow(int segments)
{
    m_GrowthPending += segments;
}

void Snake::SetDirection(Utils::Direction newDirection)
{
    if (!Utils::IsOppositeDirection(m_CurrentDirection, newDirection))
    {
        m_NextDirection = newDirection;
    }
}

sf::Vector2i Snake::GetHeadPosition() const
{
    return m_Body.empty() ? sf::Vector2i(0, 0) : m_Body[0];
}

Utils::Direction Snake::GetDirection() const
{
    return m_CurrentDirection;
}

bool Snake::CheckSelfCollision() const
{
    if (m_Body.size() < 2)
    {
        return false;
    }

    sf::Vector2i head = m_Body[0];
    
    for (size_t i = 1; i < m_Body.size(); ++i)
    {
        if (m_Body[i] == head)
        {
            return true;
        }
    }
    
    return false;
}

bool Snake::IsPositionOnSnake(sf::Vector2i position) const
{
    for (const auto& segment : m_Body)
    {
        if (segment == position)
        {
            return true;
        }
    }
    return false;
}

int Snake::GetLength() const
{
    return static_cast<int>(m_Body.size());
}

bool Snake::LoadTextures()
{
    bool headLoaded = m_HeadTexture.loadFromFile(GameConfig::SnakeHeadTexturePath);
    assert(headLoaded && "Failed to load snake head texture");
    
    bool bodyLoaded = m_BodyTexture.loadFromFile(GameConfig::SnakeBodyTexturePath);
    assert(bodyLoaded && "Failed to load snake body texture");

    if (headLoaded && bodyLoaded)
    {
        m_HeadSprite.setTexture(m_HeadTexture);
        m_BodySprite.setTexture(m_BodyTexture);

        sf::Vector2u headSize = m_HeadTexture.getSize();
        sf::Vector2u bodySize = m_BodyTexture.getSize();

        m_HeadSprite.setScale(
            static_cast<float>(GameConfig::GridSize) / headSize.x,
            static_cast<float>(GameConfig::GridSize) / headSize.y
        );

        m_BodySprite.setScale(
            static_cast<float>(GameConfig::GridSize) / bodySize.x,
            static_cast<float>(GameConfig::GridSize) / bodySize.y
        );

        m_HeadSprite.setOrigin(headSize.x / 2.0f, headSize.y / 2.0f);
        m_HeadSprite.setPosition(
            m_HeadSprite.getPosition().x + GameConfig::GridSize / 2.0f,
            m_HeadSprite.getPosition().y + GameConfig::GridSize / 2.0f
        );

        return true;
    }

    return false;
}

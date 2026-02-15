#include "Snake.h"
#include "GameConfig.h"

Snake::Snake()
    : m_Direction(Utils::Direction::Right)
    , m_NextDirection(Utils::Direction::Right)
    , m_MoveTimer(0.0f)
{
}

Snake::~Snake()
{
}

void Snake::Initialize(const sf::Vector2i& startPos, int initialLength)
{
    m_Body.clear();
    m_Direction = Utils::Direction::Right;
    m_NextDirection = Utils::Direction::Right;
    m_MoveTimer = 0.0f;

    // Add initial segments
    for (int i = 0; i < initialLength; ++i)
    {
        m_Body.push_back(sf::Vector2i(startPos.x - i, startPos.y));
    }
}

void Snake::Reset()
{
    m_Body.clear();
    m_Direction = Utils::Direction::Right;
    m_NextDirection = Utils::Direction::Right;
    m_MoveTimer = 0.0f;

    // Starting position in the center
    sf::Vector2i startPos(GameConfig::FieldWidth / 2, GameConfig::FieldHeight / 2);

    // Add initial segments
    for (int i = 0; i < GameConfig::InitialSnakeLength; ++i)
    {
        m_Body.push_back(sf::Vector2i(startPos.x - i, startPos.y));
    }
}

void Snake::SetDirection(Utils::Direction dir)
{
    // Prevent 180 degree turns
    if ((m_Direction == Utils::Direction::Up && dir == Utils::Direction::Down) ||
        (m_Direction == Utils::Direction::Down && dir == Utils::Direction::Up) ||
        (m_Direction == Utils::Direction::Left && dir == Utils::Direction::Right) ||
        (m_Direction == Utils::Direction::Right && dir == Utils::Direction::Left))
    {
        return;
    }

    m_NextDirection = dir;
}

void Snake::Update(float deltaTime, float speed)
{
    m_MoveTimer += deltaTime;
    float moveInterval = 1.0f / speed;

    if (m_MoveTimer >= moveInterval)
    {
        m_MoveTimer -= moveInterval;
        Move();
    }
}

void Snake::Move()
{
    m_Direction = m_NextDirection;

    sf::Vector2i newHead = m_Body.front();

    switch (m_Direction)
    {
    case Utils::Direction::Up:    newHead.y--; break;
    case Utils::Direction::Down:  newHead.y++; break;
    case Utils::Direction::Left:  newHead.x--; break;
    case Utils::Direction::Right: newHead.x++; break;
    }

    m_Body.insert(m_Body.begin(), newHead);
    m_Body.pop_back();
}

void Snake::Grow()
{
    // Add segments at the tail using GameConfig value
    for (int i = 0; i < GameConfig::SnakeGrowthAmount; ++i)
    {
        m_Body.push_back(m_Body.back());
    }
}

bool Snake::CheckSelfCollision() const
{
    const sf::Vector2i& head = m_Body.front();

    for (size_t i = 1; i < m_Body.size(); ++i)
    {
        if (m_Body[i] == head)
        {
            return true;
        }
    }

    return false;
}

bool Snake::CheckWallCollision() const
{
    const sf::Vector2i& head = m_Body.front();

    return head.x < 0 || head.x >= GameConfig::FieldWidth ||
        head.y < 0 || head.y >= GameConfig::FieldHeight;
}

bool Snake::IsPositionOnSnake(const sf::Vector2i& position) const
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

sf::Vector2i Snake::GetHeadPosition() const
{
    return m_Body.front();
}

const std::vector<sf::Vector2i>& Snake::GetBody() const
{
    return m_Body;
}

Utils::Direction Snake::GetDirection() const
{
    return m_Direction;
}

void Snake::Draw(sf::RenderWindow& window)
{
    for (size_t i = 0; i < m_Body.size(); ++i)
    {
        const sf::Vector2i& gridPos = m_Body[i];

        sf::Vector2f screenPos(
            static_cast<float>(GameConfig::FieldOffsetX + gridPos.x * GameConfig::GridSize),
            static_cast<float>(GameConfig::FieldOffsetY + gridPos.y * GameConfig::GridSize)
        );

        if (i == 0) // Head
        {
            // Компенсация origin: добавляем половину размера клетки
            sf::Vector2f headPos = screenPos;
            headPos.x += static_cast<float>(GameConfig::GridSize) / 2.0f;
            headPos.y += static_cast<float>(GameConfig::GridSize) / 2.0f;

            m_HeadSprite.setPosition(headPos);

            // Set rotation based on direction
            float rotation = 0.0f;
            switch (m_Direction)
            {
            case Utils::Direction::Right: rotation = 0.0f; break;
            case Utils::Direction::Down:  rotation = 90.0f; break;
            case Utils::Direction::Left:  rotation = 180.0f; break;
            case Utils::Direction::Up:    rotation = 270.0f; break;
            }
            m_HeadSprite.setRotation(rotation);

            window.draw(m_HeadSprite);
        }
        else // Body
        {
            m_BodySprite.setPosition(screenPos);
            window.draw(m_BodySprite);
        }
    }
}

bool Snake::LoadTextures()
{
    if (!m_HeadTexture.loadFromFile(GameConfig::SnakeHeadTexturePath))
    {
        return false;
    }

    if (!m_BodyTexture.loadFromFile(GameConfig::SnakeBodyTexturePath))
    {
        return false;
    }

    m_HeadSprite.setTexture(m_HeadTexture);
    m_BodySprite.setTexture(m_BodyTexture);

    // Set origin to center for proper rotation
    sf::Vector2f headSize(
        static_cast<float>(m_HeadTexture.getSize().x),
        static_cast<float>(m_HeadTexture.getSize().y)
    );
    m_HeadSprite.setOrigin(headSize.x / 2.0f, headSize.y / 2.0f);

    // Scale textures to fit grid size
    float scaleX = static_cast<float>(GameConfig::GridSize) / headSize.x;
    float scaleY = static_cast<float>(GameConfig::GridSize) / headSize.y;
    m_HeadSprite.setScale(scaleX, scaleY);

    sf::Vector2f bodySize(
        static_cast<float>(m_BodyTexture.getSize().x),
        static_cast<float>(m_BodyTexture.getSize().y)
    );
    scaleX = static_cast<float>(GameConfig::GridSize) / bodySize.x;
    scaleY = static_cast<float>(GameConfig::GridSize) / bodySize.y;
    m_BodySprite.setScale(scaleX, scaleY);

    return true;
}
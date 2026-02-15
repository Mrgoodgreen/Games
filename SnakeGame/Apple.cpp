#include "Apple.h"
#include "GameConfig.h"
#include <cassert>

Apple::Apple()
    : m_Position(0, 0)
{
}

Apple::~Apple()
{
}

void Apple::SetPosition(sf::Vector2i position)
{
    m_Position = position;
}

sf::Vector2i Apple::GetPosition() const
{
    return m_Position;
}

void Apple::Draw(sf::RenderWindow& window)
{
    sf::Vector2f screenPos(
        static_cast<float>(GameConfig::FieldOffsetX + m_Position.x * GameConfig::GridSize),
        static_cast<float>(GameConfig::FieldOffsetY + m_Position.y * GameConfig::GridSize)
    );

    m_Sprite.setPosition(screenPos);
    window.draw(m_Sprite);
}

bool Apple::LoadTexture()
{
    bool loaded = m_Texture.loadFromFile(GameConfig::AppleTexturePath);
    assert(loaded && "Failed to load apple texture");

    if (loaded)
    {
        m_Sprite.setTexture(m_Texture);

        sf::Vector2u textureSize = m_Texture.getSize();
        m_Sprite.setScale(
            static_cast<float>(GameConfig::GridSize) / static_cast<float>(textureSize.x),
            static_cast<float>(GameConfig::GridSize) / static_cast<float>(textureSize.y)
        );

        return true;
    }

    return false;
}
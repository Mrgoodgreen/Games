#include "Brick.h"

namespace ArkanoidGame
{
    Brick::Brick(int durability)
        : m_Durability(durability), m_MaxDurability(durability), m_Texture(nullptr), m_BaseColor(sf::Color::White)
    {
    }

    void Brick::Initialize(float width, float height, float x, float y)
    {
        m_Shape.setSize(sf::Vector2f(width, height));
        m_Shape.setOrigin(width / 2.f, height / 2.f);
        m_Shape.setPosition(x, y);
        UpdateVisualState();
    }

    void Brick::SetTexture(const sf::Texture* texture)
    {
        m_Texture = texture;
        m_Shape.setTexture(texture);
    }

    void Brick::SetFillColor(const sf::Color& color)
    {
        m_BaseColor = color;
        UpdateVisualState();
    }

    void Brick::SetOutline(const sf::Color& color, float thickness)
    {
        m_Shape.setOutlineColor(color);
        m_Shape.setOutlineThickness(thickness);
    }

    bool Brick::OnHit()
    {
        if (m_Durability > 0)
        {
            --m_Durability;
            UpdateVisualState();
        }

        return IsDestroyed();
    }

    void Brick::SetDurability(int durability)
    {
        if (durability < 0)
            durability = 0;

        m_Durability = durability;
        if (m_Durability > m_MaxDurability)
            m_MaxDurability = m_Durability;

        UpdateVisualState();
    }

    void Brick::UpdateVisualState()
    {
        m_Shape.setFillColor(m_BaseColor);
    }

    sf::Shape& Brick::GetShape()
    {
        return m_Shape;
    }

    const sf::Shape& Brick::GetShape() const
    {
        return m_Shape;
    }

    StrongBrick::StrongBrick()
        : Brick(3)
    {
    }

    void StrongBrick::UpdateVisualState()
    {
        if (m_Durability >= 3)
        {
            m_Shape.setFillColor(sf::Color(120, 80, 255));
        }
        else if (m_Durability == 2)
        {
            m_Shape.setFillColor(sf::Color(255, 210, 120));
        }
        else if (m_Durability == 1)
        {
            m_Shape.setFillColor(sf::Color(255, 120, 120));
        }
        else
        {
            m_Shape.setFillColor(sf::Color::Transparent);
        }
    }

    GlassBrick::GlassBrick()
        : Brick(1)
    {
    }

    void GlassBrick::UpdateVisualState()
    {
        if (m_Durability > 0)
        {
            m_Shape.setFillColor(sf::Color(255, 255, 255, 100));
        }
        else
        {
            m_Shape.setFillColor(sf::Color::Transparent);
        }
    }
}

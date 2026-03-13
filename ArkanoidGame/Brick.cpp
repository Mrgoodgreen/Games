#include "Brick.h"

namespace ArkanoidGame
{
    Brick::Brick()
    {
    }

    void Brick::Initialize(float width, float height, float x, float y)
    {
        m_Shape.setSize(sf::Vector2f(width, height));
        m_Shape.setOrigin(width / 2.f, height / 2.f);
        m_Shape.setPosition(x, y);
    }

    void Brick::SetTexture(const sf::Texture* texture)
    {
        m_Shape.setTexture(texture);
    }

    void Brick::SetFillColor(const sf::Color& color)
    {
        m_Shape.setFillColor(color);
    }

    void Brick::SetOutline(const sf::Color& color, float thickness)
    {
        m_Shape.setOutlineColor(color);
        m_Shape.setOutlineThickness(thickness);
    }

    sf::Shape& Brick::GetShape()
    {
        return m_Shape;
    }

    const sf::Shape& Brick::GetShape() const
    {
        return m_Shape;
    }
}

#include "GameObject.h"

namespace ArkanoidGame
{
    GameObject::GameObject()
        : m_Velocity(0.f, 0.f)
    {
    }

    void GameObject::Draw(sf::RenderWindow& window) const
    {
        window.draw(GetShape());
    }

    void GameObject::SetPosition(float x, float y)
    {
        GetShape().setPosition(x, y);
    }

    sf::Vector2f GameObject::GetPosition() const
    {
        return GetShape().getPosition();
    }

    sf::FloatRect GameObject::GetBounds() const
    {
        return GetShape().getGlobalBounds();
    }
}

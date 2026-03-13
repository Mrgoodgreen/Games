#pragma once

#include <SFML/Graphics.hpp>

namespace ArkanoidGame
{
    class GameObject
    {
    public:
        virtual ~GameObject() = default;

        void Draw(sf::RenderWindow& window) const;
        void SetPosition(float x, float y);
        sf::Vector2f GetPosition() const;
        sf::FloatRect GetBounds() const;

    protected:
        GameObject();

        sf::Vector2f m_Velocity;

    private:
        virtual sf::Shape& GetShape() = 0;
        virtual const sf::Shape& GetShape() const = 0;
    };
}

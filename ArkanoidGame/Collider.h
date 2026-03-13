#pragma once
#include <SFML/Graphics.hpp>

namespace ArkanoidGame
{
    class Collider
    {
    public:
        virtual ~Collider() = default;

        virtual sf::FloatRect GetBounds() const = 0;

        bool Intersects(const Collider& other) const
        {
            return GetBounds().intersects(other.GetBounds());
        }
    };
}

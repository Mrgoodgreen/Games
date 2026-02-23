#pragma once
#include <SFML/Graphics.hpp>
#include "GameSettings.h"

namespace ArkanoidGame
{
    class Platform
    {
    public:
        Platform();
        ~Platform();

        void Initialize(float width, float height, float x, float y);
        void Update(float timeDelta);
        void Draw(sf::RenderWindow& window) const;

        void SetPosition(float x, float y);
        void SetVelocity(float vx);
        void MoveLeft(float speed);
        void MoveRight(float speed);
        void Stop();

        sf::Vector2f GetPosition() const;
        sf::FloatRect GetBounds() const;
        float GetWidth() const;
        float GetHeight() const;

        void ConstrainToPlayField(const sf::FloatRect& playField);

    private:
        sf::RectangleShape m_Shape;
        sf::Vector2f m_Velocity;
        float m_MaxSpeed;

        void UpdatePosition(float timeDelta);
    };
}

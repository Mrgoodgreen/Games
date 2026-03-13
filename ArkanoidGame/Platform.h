#pragma once
#include <SFML/Graphics.hpp>
#include "GameSettings.h"
#include "GameObject.h"

namespace ArkanoidGame
{
    class Platform : public GameObject
    {
    public:
        Platform();
        ~Platform();

        void Initialize(float width, float height, float x, float y);
        void Update(float timeDelta);

        void SetVelocity(float vx);
        void MoveLeft(float speed);
        void MoveRight(float speed);
        void Stop();

        float GetWidth() const;
        float GetHeight() const;

        void ConstrainToPlayField(const sf::FloatRect& playField);

    private:
        sf::RectangleShape m_Shape;
        float m_MaxSpeed;

        void UpdatePosition(float timeDelta);

        sf::Shape& GetShape() override;
        const sf::Shape& GetShape() const override;
    };
}

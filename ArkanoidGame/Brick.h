#pragma once
#include "GameObject.h"

namespace ArkanoidGame
{
    class Brick : public GameObject
    {
    public:
        Brick();
        ~Brick() = default;

        void Initialize(float width, float height, float x, float y);
        void Update(float /*timeDelta*/) {}

        void SetTexture(const sf::Texture* texture);
        void SetFillColor(const sf::Color& color);
        void SetOutline(const sf::Color& color, float thickness);

    private:
        sf::RectangleShape m_Shape;

        sf::Shape& GetShape() override;
        const sf::Shape& GetShape() const override;
    };
}

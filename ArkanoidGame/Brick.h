#pragma once
#include "GameObject.h"

namespace ArkanoidGame
{
    class Brick : public GameObject
    {
    public:
        Brick(int durability = 1);
        virtual ~Brick() = default;

        virtual void Initialize(float width, float height, float x, float y);
        virtual void Update(float /*timeDelta*/) {}

        void SetTexture(const sf::Texture* texture);
        virtual void SetFillColor(const sf::Color& color);
        void SetOutline(const sf::Color& color, float thickness);

        virtual bool OnHit();
        virtual bool ShouldReflectBall() const { return true; }
        bool IsDestroyed() const { return m_Durability <= 0; }

    protected:
        virtual void UpdateVisualState();

        sf::RectangleShape m_Shape;
        int m_Durability;
        int m_MaxDurability;
        const sf::Texture* m_Texture;
        sf::Color m_BaseColor;

    private:
        sf::Shape& GetShape() override;
        const sf::Shape& GetShape() const override;
    };

    class StrongBrick : public Brick
    {
    public:
        StrongBrick();

    protected:
        void UpdateVisualState() override;
    };

    class GlassBrick : public Brick
    {
    public:
        GlassBrick();

        bool ShouldReflectBall() const override { return false; }

    protected:
        void UpdateVisualState() override;
    };
}

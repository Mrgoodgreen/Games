#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <cstdlib>
#include "GameObject.h"

namespace ArkanoidGame
{
    struct BonusRuntimeContext
    {
        sf::RectangleShape& platform;
        sf::Vector2f& ballVelocity;
        float& ballSpeed;
        int& scoreMultiplier;
    };

    class IBonusEffect
    {
    public:
        virtual ~IBonusEffect() = default;
        virtual void Apply(BonusRuntimeContext& context) = 0;
        virtual void Revert(BonusRuntimeContext& context) = 0;
    };

    class ExpandPlatformEffect : public IBonusEffect
    {
    public:
        void Apply(BonusRuntimeContext& context) override
        {
            sf::Vector2f size = context.platform.getSize();
            context.platform.setSize(sf::Vector2f(size.x * 1.5f, size.y));
            context.platform.setOrigin(context.platform.getSize().x / 2.f, context.platform.getSize().y / 2.f);
        }

        void Revert(BonusRuntimeContext& context) override
        {
            sf::Vector2f size = context.platform.getSize();
            context.platform.setSize(sf::Vector2f(size.x / 1.5f, size.y));
            context.platform.setOrigin(context.platform.getSize().x / 2.f, context.platform.getSize().y / 2.f);
        }
    };

    class SlowBallEffect : public IBonusEffect
    {
    public:
        void Apply(BonusRuntimeContext& context) override
        {
            context.ballSpeed *= 0.75f;
            context.ballVelocity *= 0.75f;
        }

        void Revert(BonusRuntimeContext& context) override
        {
            context.ballSpeed /= 0.75f;
            context.ballVelocity /= 0.75f;
        }
    };

    class DoubleScoreEffect : public IBonusEffect
    {
    public:
        void Apply(BonusRuntimeContext& context) override
        {
            context.scoreMultiplier *= 2;
        }

        void Revert(BonusRuntimeContext& context) override
        {
            context.scoreMultiplier /= 2;
            if (context.scoreMultiplier < 1)
                context.scoreMultiplier = 1;
        }
    };

    enum class BonusType
    {
        ExpandPlatform,
        SlowBall,
        DoubleScore
    };

    class Bonus : public GameObject
    {
    public:
        Bonus(BonusType type, float durationSec, std::unique_ptr<IBonusEffect> effect)
            : m_Type(type), m_DurationSec(durationSec), m_Effect(std::move(effect))
        {
            m_Shape.setSize(sf::Vector2f(24.f, 24.f));
            m_Shape.setOrigin(12.f, 12.f);
            m_Velocity = sf::Vector2f(0.f, 180.f);

            switch (m_Type)
            {
            case BonusType::ExpandPlatform:
                m_Shape.setFillColor(sf::Color(120, 220, 255));
                break;
            case BonusType::SlowBall:
                m_Shape.setFillColor(sf::Color(120, 255, 160));
                break;
            case BonusType::DoubleScore:
                m_Shape.setFillColor(sf::Color(255, 220, 120));
                break;
            }
        }

        void Update(float timeDelta)
        {
            m_Shape.move(m_Velocity * timeDelta);
        }

        float GetDuration() const { return m_DurationSec; }

        void Apply(BonusRuntimeContext& context)
        {
            if (m_Effect)
            {
                m_Effect->Apply(context);
            }
        }

        void Revert(BonusRuntimeContext& context)
        {
            if (m_Effect)
            {
                m_Effect->Revert(context);
            }
        }

    private:
        BonusType m_Type;
        float m_DurationSec;
        std::unique_ptr<IBonusEffect> m_Effect;
        sf::RectangleShape m_Shape;

        sf::Shape& GetShape() override { return m_Shape; }
        const sf::Shape& GetShape() const override { return m_Shape; }
    };

    class BonusFactory
    {
    public:
        static std::unique_ptr<Bonus> TryCreate(const sf::Vector2f& spawnPosition)
        {
            if ((std::rand() % 100) >= 10)
            {
                return nullptr;
            }

            const int duration = 5 + (std::rand() % 6);
            const int randomType = std::rand() % 3;

            std::unique_ptr<Bonus> bonus;
            if (randomType == 0)
            {
                bonus = std::make_unique<Bonus>(BonusType::ExpandPlatform, static_cast<float>(duration), std::make_unique<ExpandPlatformEffect>());
            }
            else if (randomType == 1)
            {
                bonus = std::make_unique<Bonus>(BonusType::SlowBall, static_cast<float>(duration), std::make_unique<SlowBallEffect>());
            }
            else
            {
                bonus = std::make_unique<Bonus>(BonusType::DoubleScore, static_cast<float>(duration), std::make_unique<DoubleScoreEffect>());
            }

            bonus->SetPosition(spawnPosition.x, spawnPosition.y);
            return bonus;
        }
    };
}

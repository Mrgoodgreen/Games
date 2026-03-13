#pragma once
#include <SFML/Graphics.hpp>
#include "GameSettings.h"
#include "GameObject.h"

namespace ArkanoidGame
{
	class Ball : public GameObject
	{
	public:
		Ball();
		~Ball();

		void Initialize(const sf::Texture& texture, float radius, float initialX, float initialY);
		void Update(float timeDelta, const sf::FloatRect& playField);

		void SetVelocity(float vx, float vy);
		
		sf::Vector2f GetVelocity() const;
		float GetRadius() const;

		bool IsMoving() const;
		void Launch(float vx, float vy);
		void Stop();

	private:
		sf::CircleShape m_Shape;
		float m_Radius;
		bool m_IsMoving;

		void HandleWallCollisions(const sf::FloatRect& playField);

		sf::Shape& GetShape() override;
		const sf::Shape& GetShape() const override;
	};
}

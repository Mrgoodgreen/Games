#pragma once
#include <SFML/Graphics.hpp>
#include "GameSettings.h"

namespace ArkanoidGame
{
	class Ball
	{
	public:
		Ball();
		~Ball();

		void Initialize(const sf::Texture& texture, float radius, float initialX, float initialY);
		void Update(float timeDelta, const sf::FloatRect& playField);
		void Draw(sf::RenderWindow& window) const;

		void SetVelocity(float vx, float vy);
		void SetPosition(float x, float y);
		
		sf::Vector2f GetPosition() const;
		sf::Vector2f GetVelocity() const;
		float GetRadius() const;
		sf::FloatRect GetBounds() const;

		bool IsMoving() const;
		void Launch(float vx, float vy);
		void Stop();

	private:
		sf::CircleShape m_Shape;
		sf::Vector2f m_Velocity;
		float m_Radius;
		bool m_IsMoving;

		void HandleWallCollisions(const sf::FloatRect& playField);
	};
}

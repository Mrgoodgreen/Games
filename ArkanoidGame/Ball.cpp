#include "Ball.h"
#include <cmath>

namespace ArkanoidGame
{
	Ball::Ball()
		: m_Velocity(0.f, 0.f), m_Radius(0.f), m_IsMoving(false)
	{
	}

	Ball::~Ball()
	{
	}

	void Ball::Initialize(const sf::Texture& texture, float radius, float initialX, float initialY)
	{
		m_Radius = radius;
		m_Shape.setRadius(radius);
		m_Shape.setOrigin(radius, radius);
		m_Shape.setPosition(initialX, initialY);
		m_Shape.setFillColor(sf::Color::White);
		m_IsMoving = false;
		m_Velocity = sf::Vector2f(0.f, 0.f);
	}

	void Ball::Update(float timeDelta, const sf::FloatRect& playField)
	{
		if (!m_IsMoving)
			return;

		sf::Vector2f newPosition = m_Shape.getPosition() + m_Velocity * timeDelta;
		m_Shape.setPosition(newPosition);

		HandleWallCollisions(playField);
	}

	void Ball::Draw(sf::RenderWindow& window) const
	{
		window.draw(m_Shape);
	}

	void Ball::SetVelocity(float vx, float vy)
	{
		m_Velocity = sf::Vector2f(vx, vy);
	}

	void Ball::SetPosition(float x, float y)
	{
		m_Shape.setPosition(x, y);
	}

	sf::Vector2f Ball::GetPosition() const
	{
		return m_Shape.getPosition();
	}

	sf::Vector2f Ball::GetVelocity() const
	{
		return m_Velocity;
	}

	float Ball::GetRadius() const
	{
		return m_Radius;
	}

	sf::FloatRect Ball::GetBounds() const
	{
		return m_Shape.getGlobalBounds();
	}

	bool Ball::IsMoving() const
	{
		return m_IsMoving;
	}

	void Ball::Launch(float vx, float vy)
	{
		m_Velocity = sf::Vector2f(vx, vy);
		m_IsMoving = true;
	}

	void Ball::Stop()
	{
		m_Velocity = sf::Vector2f(0.f, 0.f);
		m_IsMoving = false;
	}

	void Ball::HandleWallCollisions(const sf::FloatRect& playField)
	{
		sf::Vector2f pos = m_Shape.getPosition();
		
		// Top and bottom walls
		if (pos.y - m_Radius <= playField.top)
		{
			m_Velocity.y = -m_Velocity.y;
			m_Shape.setPosition(pos.x, playField.top + m_Radius);
		}
		else if (pos.y + m_Radius >= playField.top + playField.height)
		{
			// Ball fell off bottom - will be handled by game state
		}

		// Left and right walls
		if (pos.x - m_Radius <= playField.left)
		{
			m_Velocity.x = -m_Velocity.x;
			m_Shape.setPosition(playField.left + m_Radius, pos.y);
		}
		else if (pos.x + m_Radius >= playField.left + playField.width)
		{
			m_Velocity.x = -m_Velocity.x;
			m_Shape.setPosition(playField.left + playField.width - m_Radius, pos.y);
		}
	}
}

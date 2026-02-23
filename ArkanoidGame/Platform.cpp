#include "Platform.h"
#include <algorithm>

namespace ArkanoidGame
{
	Platform::Platform()
		: m_Velocity(0.f, 0.f), m_MaxSpeed(500.f)
	{
	}

	Platform::~Platform()
	{
	}

	void Platform::Initialize(float width, float height, float x, float y)
	{
		m_Shape.setSize(sf::Vector2f(width, height));
		m_Shape.setPosition(x, y);
		m_Shape.setFillColor(sf::Color::Blue);
		m_Shape.setOrigin(width / 2.f, height / 2.f);
		m_Velocity = sf::Vector2f(0.f, 0.f);
	}

	void Platform::Update(float timeDelta)
	{
		UpdatePosition(timeDelta);
	}

	void Platform::Draw(sf::RenderWindow& window) const
	{
		window.draw(m_Shape);
	}

	void Platform::SetPosition(float x, float y)
	{
		m_Shape.setPosition(x, y);
	}

	void Platform::SetVelocity(float vx)
	{
		m_Velocity.x = std::max(-m_MaxSpeed, std::min(m_MaxSpeed, vx));
	}

	void Platform::MoveLeft(float speed)
	{
		m_Velocity.x = -speed;
	}

	void Platform::MoveRight(float speed)
	{
		m_Velocity.x = speed;
	}

	void Platform::Stop()
	{
		m_Velocity.x = 0.f;
	}

	sf::Vector2f Platform::GetPosition() const
	{
		return m_Shape.getPosition();
	}

	sf::FloatRect Platform::GetBounds() const
	{
		return m_Shape.getGlobalBounds();
	}

	float Platform::GetWidth() const
	{
		return m_Shape.getSize().x;
	}

	float Platform::GetHeight() const
	{
		return m_Shape.getSize().y;
	}

	void Platform::ConstrainToPlayField(const sf::FloatRect& playField)
	{
		sf::Vector2f pos = m_Shape.getPosition();
		float halfWidth = m_Shape.getSize().x / 2.f;

		if (pos.x - halfWidth < playField.left)
		{
			m_Shape.setPosition(playField.left + halfWidth, pos.y);
		}
		else if (pos.x + halfWidth > playField.left + playField.width)
		{
			m_Shape.setPosition(playField.left + playField.width - halfWidth, pos.y);
		}
	}

	void Platform::UpdatePosition(float timeDelta)
	{
		if (m_Velocity.x != 0.f)
		{
			sf::Vector2f currentPos = m_Shape.getPosition();
			m_Shape.setPosition(currentPos + m_Velocity * timeDelta);
		}
	}
}

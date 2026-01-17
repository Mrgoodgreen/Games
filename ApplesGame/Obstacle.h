#pragma once

#include <SFML/Graphics.hpp>
#include "Math.h"

namespace ApplesGame
{
	struct Obstacle
	{
		Position position;
		sf::Sprite sprite;
	};

	void InitObstacle(Obstacle& obstacle, const sf::Texture& texture);
	void DrawObstacle(Obstacle& obstacle, sf::RenderWindow& window);
}

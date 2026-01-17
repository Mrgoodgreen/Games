#include "Obstacle.h"
#include "GameSettings.h"
#include <cmath>

namespace ApplesGame
{
	void InitObstacle(Obstacle& obstacle, const sf::Texture& texture)
	{
		// Init obstacle state with random position
		obstacle.position.x = (float)(rand() % (SCREEN_WIDTH + 1));
		obstacle.position.y = (float)(rand() % (SCREEN_HEGHT + 1));

		// Init sprite
		obstacle.sprite.setTexture(texture);
		obstacle.sprite.setOrigin(GetSpriteOrigin(obstacle.sprite, { 0.5f, 0.5f }));
		obstacle.sprite.setScale(GetSpriteScale(obstacle.sprite, { OBSTACLE_SIZE, OBSTACLE_SIZE }));
	}

	void DrawObstacle(Obstacle& obstacle, sf::RenderWindow& window)
	{
		obstacle.sprite.setPosition(OurVectorToSf(obstacle.position));
		window.draw(obstacle.sprite);
	}
}

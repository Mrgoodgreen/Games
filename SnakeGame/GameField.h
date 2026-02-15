#pragma once
#include <SFML/Graphics.hpp>

class GameField
{
public:
    GameField();
    ~GameField();

    void Draw(sf::RenderWindow& window);
    bool IsPositionValid(sf::Vector2i position) const;
    bool IsWallCollision(sf::Vector2i position) const;

private:
    void InitializeWalls();
    
    sf::RectangleShape m_GridCell;
    sf::RectangleShape m_Wall;
};

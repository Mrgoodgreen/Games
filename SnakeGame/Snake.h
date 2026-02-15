#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Utils.h"

class Snake
{
public:
    Snake();
    ~Snake();

    void Initialize(sf::Vector2i startPosition, int length);
    void Update(float deltaTime, float speed);
    void Draw(sf::RenderWindow& window);
    void Grow(int segments);
    void SetDirection(Utils::Direction newDirection);
    
    sf::Vector2i GetHeadPosition() const;
    Utils::Direction GetDirection() const;
    bool CheckSelfCollision() const;
    bool IsPositionOnSnake(sf::Vector2i position) const;
    int GetLength() const;

    bool LoadTextures();

private:
    std::vector<sf::Vector2i> m_Body;
    Utils::Direction m_CurrentDirection;
    Utils::Direction m_NextDirection;
    float m_MoveTimer;
    int m_GrowthPending;

    sf::Texture m_HeadTexture;
    sf::Texture m_BodyTexture;
    sf::Sprite m_HeadSprite;
    sf::Sprite m_BodySprite;
};

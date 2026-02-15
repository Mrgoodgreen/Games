#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Utils.h"

class Snake
{
public:
    Snake();
    ~Snake();

    // Initialization and reset
    void Initialize(const sf::Vector2i& startPos, int initialLength);
    void Reset();

    // Movement and direction
    void SetDirection(Utils::Direction dir);
    void Update(float deltaTime, float speed);
    void Move();
    void Grow();

    // Collision detection
    bool CheckSelfCollision() const;
    bool CheckWallCollision() const;
    bool IsPositionOnSnake(const sf::Vector2i& position) const;

    // Getters
    sf::Vector2i GetHeadPosition() const;
    const std::vector<sf::Vector2i>& GetBody() const;
    Utils::Direction GetDirection() const;

    // Rendering
    void Draw(sf::RenderWindow& window);
    bool LoadTextures();

private:
    std::vector<sf::Vector2i> m_Body;
    Utils::Direction m_Direction;
    Utils::Direction m_NextDirection;

    float m_MoveTimer;

    sf::Texture m_HeadTexture;
    sf::Texture m_BodyTexture;
    sf::Sprite m_HeadSprite;
    sf::Sprite m_BodySprite;
};
#pragma once
#include <SFML/Graphics.hpp>

class Apple
{
public:
    Apple();
    ~Apple();

    void SetPosition(sf::Vector2i position);
    sf::Vector2i GetPosition() const;
    void Draw(sf::RenderWindow& window);
    bool LoadTexture();

private:
    sf::Vector2i m_Position;
    sf::Texture m_Texture;
    sf::Sprite m_Sprite;
};

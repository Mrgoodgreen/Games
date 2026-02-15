#include "GameField.h"
#include "GameConfig.h"

GameField::GameField()
{
    InitializeWalls();
}

GameField::~GameField()
{
}

void GameField::InitializeWalls()
{
    m_GridCell.setSize(sf::Vector2f(
        static_cast<float>(GameConfig::GridSize),
        static_cast<float>(GameConfig::GridSize)
    ));
    m_GridCell.setFillColor(sf::Color::Transparent);
    m_GridCell.setOutlineColor(GameConfig::GridColor);
    m_GridCell.setOutlineThickness(1.0f);

    m_Wall.setSize(sf::Vector2f(
        static_cast<float>(GameConfig::GridSize),
        static_cast<float>(GameConfig::GridSize)
    ));
    m_Wall.setFillColor(GameConfig::WallColor);
}

void GameField::Draw(sf::RenderWindow& window)
{
    for (int y = 0; y < GameConfig::FieldHeight; ++y)
    {
        for (int x = 0; x < GameConfig::FieldWidth; ++x)
        {
            sf::Vector2f position(
                GameConfig::FieldOffsetX + x * GameConfig::GridSize,
                GameConfig::FieldOffsetY + y * GameConfig::GridSize
            );

            bool isWall = (x == 0 || x == GameConfig::FieldWidth - 1 || 
                          y == 0 || y == GameConfig::FieldHeight - 1);

            if (isWall)
            {
                m_Wall.setPosition(position);
                window.draw(m_Wall);
            }
            else
            {
                m_GridCell.setPosition(position);
                window.draw(m_GridCell);
            }
        }
    }
}

bool GameField::IsPositionValid(sf::Vector2i position) const
{
    return position.x >= 0 && position.x < GameConfig::FieldWidth &&
           position.y >= 0 && position.y < GameConfig::FieldHeight;
}

bool GameField::IsWallCollision(sf::Vector2i position) const
{
    return position.x <= 0 || position.x >= GameConfig::FieldWidth - 1 ||
           position.y <= 0 || position.y >= GameConfig::FieldHeight - 1;
}

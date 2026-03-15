#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace ArkanoidGame
{
    enum class BrickType
    {
        Normal,
        Strong,
        Glass
    };

    struct BrickSnapshot
    {
        BrickType type = BrickType::Normal;
        sf::Vector2f position;
        int durability = 1;
    };

    struct PlayingMemento
    {
        std::vector<BrickSnapshot> bricks;
        int score = 0;
    };
}

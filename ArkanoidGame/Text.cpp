#include "Text.h"
#include <algorithm>

namespace ArkanoidGame
{
    sf::Vector2f GetTextOrigin(const sf::Text& text, const sf::Vector2f& relativePosition)
    {
        sf::FloatRect textSize = text.getLocalBounds();
        return {
            (textSize.left + textSize.width) * relativePosition.x,
            (textSize.top + textSize.height) * relativePosition.y,
        };
    }

    void DrawTextList(sf::RenderWindow& window, const std::vector<sf::Text*>& items, float spacing,
        Orientation orientation, Alignment alignment, const sf::Vector2f& position,
        const sf::Vector2f& origin)
    {
        if (items.empty()) {
            return;
        }

        sf::Vector2f totalSize(0.f, 0.f);
        for (auto* item : items) {
            sf::FloatRect bounds = item->getLocalBounds();
            totalSize.x = std::max(totalSize.x, bounds.width);
            totalSize.y += bounds.height;
        }
        totalSize.y += spacing * (items.size() - 1);

        sf::Vector2f startPos = position;
        if (orientation == Orientation::Vertical) {
            if (alignment == Alignment::Middle) {
                startPos.y -= totalSize.y / 2.f;
            }
            else if (alignment == Alignment::Max) {
                startPos.y -= totalSize.y;
            }
        }
        else { // Horizontal
            if (alignment == Alignment::Middle) {
                startPos.x -= totalSize.x / 2.f;
            }
            else if (alignment == Alignment::Max) {
                startPos.x -= totalSize.x;
            }
        }

        sf::Vector2f currentPos = startPos;
        for (auto* item : items) {
            sf::FloatRect bounds = item->getLocalBounds();

            if (orientation == Orientation::Vertical) {
                float xOffset = 0.f;
                if (alignment == Alignment::Middle) {
                    xOffset = (totalSize.x - bounds.width) / 2.f;
                }
                else if (alignment == Alignment::Max) {
                    xOffset = totalSize.x - bounds.width;
                }
                item->setOrigin(GetTextOrigin(*item, origin));
                item->setPosition(currentPos.x + xOffset, currentPos.y);
                currentPos.y += bounds.height + spacing;
            }
            else { // Horizontal
                float yOffset = 0.f;
                if (alignment == Alignment::Middle) {
                    yOffset = (totalSize.y - bounds.height) / 2.f;
                }
                else if (alignment == Alignment::Max) {
                    yOffset = totalSize.y - bounds.height;
                }
                item->setOrigin(GetTextOrigin(*item, origin));
                item->setPosition(currentPos.x, currentPos.y + yOffset);
                currentPos.x += bounds.width + spacing;
            }

            window.draw(*item);
        }
    }
}
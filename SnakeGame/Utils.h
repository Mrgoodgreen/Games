#pragma once
#include <SFML/Graphics.hpp>

namespace Utils
{
    // Direction enum for snake movement
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    // Game state enum
    enum class GameState
    {
        MainMenu,
        Playing,
        Paused,
        GameOver,
        DifficultySelection,
        SettingsMenu,
        ScoreTable
    };

    // Menu selection enum
    enum class MenuOption
    {
        StartGame,
        Difficulty,
        ScoreTable,
        Settings,
        Exit
    };

    // Settings menu options
    enum class SettingsOption
    {
        Sound,
        Music,
        Back
    };

    // Pause menu options
    enum class PauseOption
    {
        Continue,
        ExitToMenu
    };

    // Game over menu options
    enum class GameOverOption
    {
        StartGame,
        ExitToMenu
    };

    // Helper function to check if direction is opposite
    inline bool IsOppositeDirection(Direction current, Direction newDir)
    {
        return (current == Direction::Up && newDir == Direction::Down) ||
               (current == Direction::Down && newDir == Direction::Up) ||
               (current == Direction::Left && newDir == Direction::Right) ||
               (current == Direction::Right && newDir == Direction::Left);
    }

    // Helper function to get direction vector
    inline sf::Vector2i GetDirectionVector(Direction dir)
    {
        switch (dir)
        {
            case Direction::Up:    return sf::Vector2i(0, -1);
            case Direction::Down:  return sf::Vector2i(0, 1);
            case Direction::Left:  return sf::Vector2i(-1, 0);
            case Direction::Right: return sf::Vector2i(1, 0);
            default:               return sf::Vector2i(0, 0);
        }
    }
}

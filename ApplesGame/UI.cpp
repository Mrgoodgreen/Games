#include "UI.h"
#include "Game.h"
#include "Math.h"

namespace ApplesGame
{
    void InitUI(UIState& uiState, const sf::Font& font)
    {
        // Current score text
        uiState.scoreText.setFont(font);
        uiState.scoreText.setCharacterSize(24);
        uiState.scoreText.setFillColor(sf::Color::Yellow);

        // Best score text
        uiState.bestScoreText.setFont(font);
        uiState.bestScoreText.setCharacterSize(24);
        uiState.bestScoreText.setFillColor(sf::Color::Cyan);

        // Input hint text (during play)
        uiState.inputHintText.setFont(font);
        uiState.inputHintText.setCharacterSize(18);
        uiState.inputHintText.setFillColor(sf::Color::White);
        uiState.inputHintText.setString("Use arrow keys to move");
        uiState.inputHintText.setOrigin(GetTextOrigin(uiState.inputHintText, { 1.f, 0.f }));

        // Game over text
        uiState.isGameOverTextVisible = false;
        uiState.gameOverText.setFont(font);
        uiState.gameOverText.setCharacterSize(48);
        uiState.gameOverText.setStyle(sf::Text::Bold);
        uiState.gameOverText.setFillColor(sf::Color::Red);
        uiState.gameOverText.setString("GAME OVER");
        uiState.gameOverText.setOrigin(GetTextOrigin(uiState.gameOverText, { 0.5f, 0.5f }));

        // Restart hint text
        uiState.restartHintText.setFont(font);
        uiState.restartHintText.setCharacterSize(20);
        uiState.restartHintText.setFillColor(sf::Color::Green);
        uiState.restartHintText.setString("Press SPACE to restart");
        uiState.restartHintText.setOrigin(GetTextOrigin(uiState.restartHintText, { 0.5f, 0.f }));

        // Start hint text (используется в меню выбора режима)
        uiState.startHintText.setFont(font);
        uiState.startHintText.setCharacterSize(24);
        uiState.startHintText.setFillColor(sf::Color::White);
        uiState.startHintText.setString("Press 1-5 to choose mode");
        uiState.startHintText.setOrigin(GetTextOrigin(uiState.startHintText, { 0.5f, 0.5f }));

        // строки таблицы лидеров
        for (int i = 0; i < 10; ++i)
        {
            uiState.leaderboardLines[i].setFont(font);
            uiState.leaderboardLines[i].setCharacterSize(20);
            uiState.leaderboardLines[i].setFillColor(sf::Color::White);
        }
    }

    void UpdateUI(UIState& uiState, const GameState& gameState, float timeDelta)
    {
        (void)timeDelta;

        uiState.scoreText.setString("Score: " + std::to_string(gameState.numEatenApples));
        uiState.bestScoreText.setString("Best: " + std::to_string(gameState.bestScore));

        uiState.isGameOverTextVisible = gameState.isGameOver;

        // формируем строки таблицы (только в Game Over)
        if (gameState.isGameOver)
        {
            int lines = static_cast<int>(gameState.leaderboard.size());
            lines = std::min(lines, 10);
            for (int i = 0; i < lines; ++i)
            {
                const Record& r = gameState.leaderboard[i];
                std::string line = std::to_string(i + 1) + ". " + r.name + " .... " + std::to_string(r.score);
                uiState.leaderboardLines[i].setString(line);
            }
            for (int i = lines; i < 10; ++i)
            {
                uiState.leaderboardLines[i].setString("");
            }
        }
    }

    void DrawUI(UIState& uiState, sf::RenderWindow& window)
    {
        uiState.scoreText.setPosition(10.f, 10.f);
        window.draw(uiState.scoreText);

        uiState.bestScoreText.setPosition(10.f, 40.f);
        window.draw(uiState.bestScoreText);

        uiState.inputHintText.setPosition(window.getSize().x - 10.f, 10.f);
        window.draw(uiState.inputHintText);

        if (uiState.isGameOverTextVisible)
        {
            uiState.gameOverText.setPosition(
                window.getSize().x / 2.f,
                window.getSize().y / 2.f - 50.f
            );
            window.draw(uiState.gameOverText);

            uiState.restartHintText.setPosition(
                window.getSize().x / 2.f,
                window.getSize().y / 2.f + 50.f
            );
            window.draw(uiState.restartHintText);

            // таблица лидеров под Game Over
            float startY = window.getSize().y / 2.f + 100.f;
            for (int i = 0; i < 10; ++i)
            {
                if (uiState.leaderboardLines[i].getString().isEmpty())
                    break;

                uiState.leaderboardLines[i].setPosition(
                    window.getSize().x / 2.f - 200.f,
                    startY + i * 24.f
                );
                window.draw(uiState.leaderboardLines[i]);
            }
        }
    }
}

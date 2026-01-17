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

        // Start hint text (используетс€ в GameMain дл€ подсказки режимов)
        uiState.startHintText.setFont(font);
        uiState.startHintText.setCharacterSize(24);
        uiState.startHintText.setFillColor(sf::Color::White);
        uiState.startHintText.setString("Press 1-5 to choose mode");
        uiState.startHintText.setOrigin(GetTextOrigin(uiState.startHintText, { 0.5f, 0.5f }));
    }

    void UpdateUI(UIState& uiState, const GameState& gameState, float timeDelta)
    {
        (void)timeDelta;

        // Score
        uiState.scoreText.setString("Score: " + std::to_string(gameState.numEatenApples));

        // Best
        uiState.bestScoreText.setString("Best: " + std::to_string(gameState.bestScore));

        // Game over Ц по флагу isGameOver
        uiState.isGameOverTextVisible = gameState.isGameOver;
    }

    void DrawUI(UIState& uiState, sf::RenderWindow& window)
    {
        // Score
        uiState.scoreText.setPosition(10.f, 10.f);
        window.draw(uiState.scoreText);

        // Best score
        uiState.bestScoreText.setPosition(10.f, 40.f);
        window.draw(uiState.bestScoreText);

        // Input hint (top-right)
        uiState.inputHintText.setPosition(window.getSize().x - 10.f, 10.f);
        window.draw(uiState.inputHintText);

        // Game Over
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
        }
    }
}

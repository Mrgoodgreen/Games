#include "Game.h"
#include "GameStatePlaying.h"
#include "GameStateMainMenu.h"
#include "GameStatePauseMenu.h"
#include "GameStateGameOver.h"
#include "GameStateRecords.h"
#include <cassert>
#include <iostream>
#include <cstring>

namespace ArkanoidGame
{
    void InitGame(Game& game)
    {
        std::cout << "[Game] Initializing..." << std::endl;
        
        // Initialize records table with default values
        game.getRecordsTable()[PLAYER_NAME] = 0;

        // Push initial state (main menu as the starting point)
        std::cout << "[Game] Pushing MainMenuState..." << std::endl;
        // Use deferred push to avoid calling onEnter during Application construction
        game.pushStateDeferred(std::make_unique<MainMenuState>());
        
        std::cout << "[Game] Initialization complete. Active states: " << game.stateStack.size() << std::endl;
    }

    void HandleWindowEvents(Game& game, sf::RenderWindow& window)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Handle window close
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Handle event in current state
            IGameState* currentStateBefore = game.getCurrentState();
            if (currentStateBefore)
            {
                currentStateBefore->handleEvent(event);
            }

            // Special handling for Escape key to go back/pause
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                // Re-fetch current state because it may have changed during handling
                IGameState* currentStateNow = game.getCurrentState();
                if (currentStateNow && std::strcmp(currentStateNow->getStateName(), "PlayingState") == 0)
                {
                    std::cout << "[Game] Pushing PauseMenuState..." << std::endl;
                    game.pushState(std::make_unique<PauseMenuState>());
                }
            }
        }
    }

    bool UpdateGame(Game& game, float timeDelta)
    {
        if (!game.hasActiveState())
        {
            return false;
        }

        IGameState* currentState = game.getCurrentState();
        if (currentState)
        {
            currentState->update(timeDelta);
        }

        return game.hasActiveState();
    }

    void DrawGame(Game& game, sf::RenderWindow& window)
    {
        if (!game.hasActiveState())
        {
            std::cout << "[DrawGame] No active state!" << std::endl;
            return;
        }

        // Draw all visible states from bottom to top of stack
        // Note: window.clear() and window.display() are handled by Application::Run()
        for (auto& state : game.stateStack)
        {
            if (state)
            {
                state->draw(window);
            }
        }
    }

    void ShutdownGame(Game& game)
    {
        // State stack will be cleared automatically by unique_ptr destructors
        std::cout << "[Game] Shutting down..." << std::endl;
    }

    bool IsEnableOptions(const Game& game, GameOptions option)
    {
        return ((std::uint8_t)game.getOptions() & (std::uint8_t)option) != (std::uint8_t)GameOptions::Empty;
    }
}
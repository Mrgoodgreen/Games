#include "Game.h"
#include <assert.h>
#include <algorithm>
#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "GameStateRecords.h"

namespace ArkanoidGame
{
    void InitGame(Game& game)
    {
        
        game.setOptions(GameOptions::Default);
        game.getRecordsTable() = {
            { "John", 100 },
            { "Jane", 200 },
            { "Alice", 300 },
            { "Bob", 400 },
            { "Clementine", 500 },
        };

        game.setGameStateChangeType(GameStateChangeType::None);
        game.setPendingGameStateType(GameStateType::None);
        game.setPendingGameStateExclusivelyVisible(false);
        SwitchGameState(game, GameStateType::MainMenu);
    }

    void HandleWindowEvents(Game& game, sf::RenderWindow& window)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (!game.getGameStateStack().empty())
            {
                HandleWindowEventGameState(game.getGameStateStack().back(), event);
            }
        }
    }

    bool UpdateGame(Game& game, float timeDelta)
    {
        if (game.getGameStateChangeType() == GameStateChangeType::Switch)
        {
            game.clearGameStateStack();
        }
        else if (game.getGameStateChangeType() == GameStateChangeType::Pop)
        {
            if (!game.getGameStateStack().empty())
            {
                game.popGameState();
            }
        }

        if (game.getPendingGameStateType() != GameStateType::None)
        {
            GameState newState(game.getPendingGameStateType(), nullptr, game.isPendingGameStateExclusivelyVisible());
            InitGameState(newState);
            game.pushGameState(std::move(newState));
        }

        game.setGameStateChangeType(GameStateChangeType::None);
        game.setPendingGameStateType(GameStateType::None);
        game.setPendingGameStateExclusivelyVisible(false);

        if (!game.getGameStateStack().empty())
        {
            UpdateGameState(game.getGameStateStack().back(), timeDelta);
            return true;
        }

        return false;
    }

    void DrawGame(Game& game, sf::RenderWindow& window)
    {
        if (!game.getGameStateStack().empty())
        {
            std::vector<GameState*> visibleGameStates;
            for (auto it = game.getGameStateStack().rbegin(); it != game.getGameStateStack().rend(); ++it)
            {
                visibleGameStates.push_back(&(*it));
                if (it->isExclusivelyVisibleState())
                {
                    break;
                }
            }

            for (auto it = visibleGameStates.rbegin(); it != visibleGameStates.rend(); ++it)
            {
                DrawGameState(**it, window);
            }
        }
    }

    void ShutdownGame(Game& game)
    {
        game.clearGameStateStack();

        game.setGameStateChangeType(GameStateChangeType::None);
        game.setPendingGameStateType(GameStateType::None);
        game.setPendingGameStateExclusivelyVisible(false);
    }

    void PushGameState(Game& game, GameStateType stateType, bool isExclusivelyVisible)
    {
        game.setPendingGameStateType(stateType);
        game.setPendingGameStateExclusivelyVisible(isExclusivelyVisible);
        game.setGameStateChangeType(GameStateChangeType::Push);
    }

    void PopGameState(Game& game)
    {
        game.setPendingGameStateType(GameStateType::None);
        game.setPendingGameStateExclusivelyVisible(false);
        game.setGameStateChangeType(GameStateChangeType::Pop);
    }

    void SwitchGameState(Game& game, GameStateType newState)
    {
        game.setPendingGameStateType(newState);
        game.setPendingGameStateExclusivelyVisible(false);
        game.setGameStateChangeType(GameStateChangeType::Switch);
    }

    void InitGameState(GameState& state)
    {
        switch (state.getType())
        {
        case GameStateType::MainMenu:
        {
            state = GameState(GameStateType::MainMenu, new GameStateMainMenuData(), false);
            InitGameStateMainMenu(*static_cast<GameStateMainMenuData*>(state.getData()));
            break;
        }
        case GameStateType::Playing:
        {
            state = GameState(GameStateType::Playing, new GameStatePlayingData(SCREEN_WIDTH, SCREEN_HEIGHT), false);
            InitGameStatePlaying(*static_cast<GameStatePlayingData*>(state.getData()));
            break;
        }
        case GameStateType::GameOver:
        {
            state = GameState(GameStateType::GameOver, new GameStateGameOverData(), false);
            InitGameStateGameOver(*static_cast<GameStateGameOverData*>(state.getData()));
            break;
        }
        case GameStateType::PauseMenu:
        {
            state = GameState(GameStateType::PauseMenu, new GameStatePauseMenuData(), false);
            InitGameStatePauseMenu(*static_cast<GameStatePauseMenuData*>(state.getData()));
            break;
        }
        case GameStateType::Records:
        {
            state = GameState(GameStateType::Records, new GameStateRecordsData(), true);
            InitGameStateRecords(*static_cast<GameStateRecordsData*>(state.getData()));
            break;
        }
        default:
            assert(false);
            break;
        }
    }

    void ShutdownGameState(GameState& state)
    {
        switch (state.getType())
        {
        case GameStateType::MainMenu:
        {
            ShutdownGameStateMainMenu(*static_cast<GameStateMainMenuData*>(state.getData()));
            delete static_cast<GameStateMainMenuData*>(state.getData());
            break;
        }
        case GameStateType::Playing:
        {
            ShutdownGameStatePlaying(*static_cast<GameStatePlayingData*>(state.getData()));
            delete static_cast<GameStatePlayingData*>(state.getData());
            break;
        }
        case GameStateType::GameOver:
        {
            ShutdownGameStateGameOver(*static_cast<GameStateGameOverData*>(state.getData()));
            delete static_cast<GameStateGameOverData*>(state.getData());
            break;
        }
        case GameStateType::PauseMenu:
        {
            ShutdownGameStatePauseMenu(*static_cast<GameStatePauseMenuData*>(state.getData()));
            delete static_cast<GameStatePauseMenuData*>(state.getData());
            break;
        }
        case GameStateType::Records:
        {
            ShutdownGameStateRecords(*static_cast<GameStateRecordsData*>(state.getData()));
            delete static_cast<GameStateRecordsData*>(state.getData());
            break;
        }
        default:
            assert(false); 
            break;
        }
        state = GameState(); 
    }

    void HandleWindowEventGameState(GameState& state, const sf::Event& event)
    {
        switch (state.getType())
        {
        case GameStateType::MainMenu:
        {
            HandleGameStateMainMenuWindowEvent(*static_cast<GameStateMainMenuData*>(state.getData()), event);
            break;
        }
        case GameStateType::Playing:
        {
            HandleGameStatePlayingWindowEvent(*static_cast<GameStatePlayingData*>(state.getData()), event);
            break;
        }
        case GameStateType::GameOver:
        {
            HandleGameStateGameOverWindowEvent(*static_cast<GameStateGameOverData*>(state.getData()), event);
            break;
        }
        case GameStateType::PauseMenu:
        {
            HandleGameStatePauseMenuWindowEvent(*static_cast<GameStatePauseMenuData*>(state.getData()), event);
            break;
        }
        case GameStateType::Records:
        {
            HandleGameStateRecordsWindowEvent(*static_cast<GameStateRecordsData*>(state.getData()), event);
            break;
        }
        default:
            assert(false);
            break;
        }
    }

    void UpdateGameState(GameState& state, float timeDelta)
    {
        switch (state.getType())
        {
        case GameStateType::MainMenu:
        {
            UpdateGameStateMainMenu(*static_cast<GameStateMainMenuData*>(state.getData()), timeDelta);
            break;
        }
        case GameStateType::Playing:
        {
            UpdateGameStatePlaying(*static_cast<GameStatePlayingData*>(state.getData()), timeDelta);
            break;
        }
        case GameStateType::GameOver:
        {
            UpdateGameStateGameOver(*static_cast<GameStateGameOverData*>(state.getData()), timeDelta);
            break;
        }
        case GameStateType::PauseMenu:
        {
            UpdateGameStatePauseMenu(*static_cast<GameStatePauseMenuData*>(state.getData()), timeDelta);
            break;
        }
        case GameStateType::Records:
        {
            UpdateGameStateRecords(*static_cast<GameStateRecordsData*>(state.getData()), timeDelta);
            break;
        }
        default:
            assert(false);
            break;
        }
    }

    void DrawGameState(GameState& state, sf::RenderWindow& window)
    {
        switch (state.getType())
        {
        case GameStateType::MainMenu:
        {
            DrawGameStateMainMenu(*static_cast<GameStateMainMenuData*>(state.getData()), window);
            break;
        }
        case GameStateType::Playing:
        {
            DrawGameStatePlaying(*static_cast<GameStatePlayingData*>(state.getData()), window);
            break;
        }
        case GameStateType::GameOver:
        {
            DrawGameStateGameOver(*static_cast<GameStateGameOverData*>(state.getData()), window);
            break;
        }
        case GameStateType::PauseMenu:
        {
            DrawGameStatePauseMenu(*static_cast<GameStatePauseMenuData*>(state.getData()), window);
            break;
        }
        case GameStateType::Records:
        {
            DrawGameStateRecords(*static_cast<GameStateRecordsData*>(state.getData()), window);
            break;
        }
        default:
            assert(false);
            break;
        }
    }

    bool IsEnableOptions(const Game& game, GameOptions option)
    {
        bool isEnable = ((std::uint8_t)game.getOptions() & (std::uint8_t)option) != (std::uint8_t)GameOptions::Empty;
        return isEnable;
    }
}
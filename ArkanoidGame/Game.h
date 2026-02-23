#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameSettings.h"
#include <unordered_map>
#include <vector>
#include <functional>

namespace ArkanoidGame
{
    // Forward declarations for game state data types
    struct GameStateMainMenuData;
    struct GameStatePlayingData;
    struct GameStateGameOverData;
    struct GameStatePauseMenuData;
    struct GameStateRecordsData;

    enum class GameOptions : std::uint8_t
    {
        InfiniteLives = 1 << 0,
        UnlimitedBalls = 1 << 1,

        Default = InfiniteLives,
        Empty = 0
    };

    enum class GameStateType
    {
        None = 0,
        MainMenu,
        Playing,
        GameOver,
        PauseMenu,
        Records,
    };

    class GameState {
    private:
        GameStateType type;
        void* data;
        bool isExclusivelyVisible;

    public:
        GameState(GameStateType type = GameStateType::None, void* data = nullptr, bool isExclusivelyVisible = false)
            : type(type), data(data), isExclusivelyVisible(isExclusivelyVisible) {
        }

        ~GameState() {
            switch (type) {
            case GameStateType::MainMenu:
                delete static_cast<GameStateMainMenuData*>(data);
                break;
            case GameStateType::Playing:
                delete static_cast<GameStatePlayingData*>(data);
                break;
            case GameStateType::GameOver:
                delete static_cast<GameStateGameOverData*>(data);
                break;
            case GameStateType::PauseMenu:
                delete static_cast<GameStatePauseMenuData*>(data);
                break;
            case GameStateType::Records:
                delete static_cast<GameStateRecordsData*>(data);
                break;
            default:
                break;
            }
            data = nullptr;
        }

        GameState(const GameState& other) = delete;
        GameState& operator=(const GameState& other) = delete;

        GameState(GameState&& other) noexcept
            : type(other.type), data(other.data), isExclusivelyVisible(other.isExclusivelyVisible) {
            other.type = GameStateType::None;
            other.data = nullptr;
            other.isExclusivelyVisible = false;
        }

        GameState& operator=(GameState&& other) noexcept {
            if (this != &other) {
                this->~GameState();

                type = other.type;
                data = other.data;
                isExclusivelyVisible = other.isExclusivelyVisible;

                other.type = GameStateType::None;
                other.data = nullptr;
                other.isExclusivelyVisible = false;
            }
            return *this;
        }

        GameStateType getType() const { return type; }
        void* getData() const { return data; }
        bool isExclusivelyVisibleState() const { return isExclusivelyVisible; }

        template<typename T>
        T* getDataAs() const {
            return static_cast<T*>(data);
        }
    };

    enum class GameStateChangeType
    {
        None,
        Push,
        Pop,
        Switch
    };

    class Game
    {
    private:
        std::vector<GameState> gameStateStack;
        GameStateChangeType gameStateChangeType = GameStateChangeType::None;
        GameStateType pendingGameStateType = GameStateType::None;
        bool pendingGameStateIsExclusivelyVisible = false;

        GameOptions options = GameOptions::Default;
        std::unordered_map<std::string, int> recordsTable;

    public:
        Game() = default;
        ~Game() = default;

        Game(const Game& other) = delete;
        Game& operator=(const Game& other) = delete;

        Game(Game&& other) noexcept = default;
        Game& operator=(Game&& other) noexcept = default;

        // Provide const and non-const access to game state stack
        const std::vector<GameState>& getGameStateStack() const { return gameStateStack; }
        std::vector<GameState>& getGameStateStack() { return gameStateStack; }

        GameStateChangeType getGameStateChangeType() const { return gameStateChangeType; }
        GameStateType getPendingGameStateType() const { return pendingGameStateType; }
        bool isPendingGameStateExclusivelyVisible() const { return pendingGameStateIsExclusivelyVisible; }
        GameOptions getOptions() const { return options; }

        // Provide const and non-const access to records table
        const std::unordered_map<std::string, int>& getRecordsTable() const { return recordsTable; }
        std::unordered_map<std::string, int>& getRecordsTable() { return recordsTable; }

        void setGameStateChangeType(GameStateChangeType type) { gameStateChangeType = type; }
        void setPendingGameStateType(GameStateType type) { pendingGameStateType = type; }
        void setPendingGameStateExclusivelyVisible(bool value) { pendingGameStateIsExclusivelyVisible = value; }
        void setOptions(GameOptions newOptions) { options = newOptions; }

        void pushGameState(GameState&& state) {
            gameStateStack.push_back(std::move(state));
        }

        void popGameState() {
            if (!gameStateStack.empty()) {
                gameStateStack.pop_back();
            }
        }

        void clearGameStateStack() {
            gameStateStack.clear();
        }
    };

    void InitGame(Game& game);
    void HandleWindowEvents(Game& game, sf::RenderWindow& window);
    bool UpdateGame(Game& game, float timeDelta);
    void DrawGame(Game& game, sf::RenderWindow& window);
    void ShutdownGame(Game& game);

    void PushGameState(Game& game, GameStateType stateType, bool isExclusivelyVisible);
    void PopGameState(Game& game);
    void SwitchGameState(Game& game, GameStateType newState);

    void InitGameState(GameState& state);
    void ShutdownGameState(GameState& state);
    void HandleWindowEventGameState(GameState& state, const sf::Event& event);
    void UpdateGameState(GameState& state, float timeDelta);
    void DrawGameState(GameState& state, sf::RenderWindow& window);
    bool IsEnableOptions(const Game& game, GameOptions option);
}
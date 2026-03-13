#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameSettings.h"
#include "IGameState.h"
#include <unordered_map>
#include <vector>
#include <memory>

namespace ArkanoidGame
{
    enum class GameOptions : std::uint8_t
    {
        InfiniteLives = 1 << 0,
        UnlimitedBalls = 1 << 1,

        Default = InfiniteLives,
        Empty = 0
    };

    // Main game class managing state stack and game loop
    class Game
    {
    public:
        std::vector<GameStatePtr> stateStack;  // Made public for DrawGame access

    private:
        GameOptions options = GameOptions::Default;
        std::unordered_map<std::string, int> recordsTable;

    public:
        Game() = default;
        ~Game() = default;

        Game(const Game& other) = delete;
        Game& operator=(const Game& other) = delete;

        Game(Game&& other) noexcept = default;
        Game& operator=(Game&& other) noexcept = default;

        // State stack operations
        void pushState(GameStatePtr state) {
            if (!stateStack.empty()) {
                stateStack.back()->onExit();
            }
            state->onEnter();
            stateStack.push_back(std::move(state));
        }

        // Push state without calling onEnter immediately (deferred activation)
        void pushStateDeferred(GameStatePtr state) {
            if (!stateStack.empty()) {
                stateStack.back()->onExit();
            }
            stateStack.push_back(std::move(state));
        }

        void popState() {
            if (!stateStack.empty()) {
                stateStack.back()->onExit();
                stateStack.pop_back();
            }
            if (!stateStack.empty()) {
                stateStack.back()->onEnter();
            }
        }

        void switchState(GameStatePtr state) {
            stateStack.clear();
            state->onEnter();
            stateStack.push_back(std::move(state));
        }

        IGameState* getCurrentState() {
            return stateStack.empty() ? nullptr : stateStack.back().get();
        }

        bool hasActiveState() const { return !stateStack.empty(); }

        // Game options
        GameOptions getOptions() const { return options; }
        void setOptions(GameOptions newOptions) { options = newOptions; }

        // Records table
        const std::unordered_map<std::string, int>& getRecordsTable() const { return recordsTable; }
        std::unordered_map<std::string, int>& getRecordsTable() { return recordsTable; }
    };

    // Free functions for game loop
    void InitGame(Game& game);
    void HandleWindowEvents(Game& game, sf::RenderWindow& window);
    bool UpdateGame(Game& game, float timeDelta);
    void DrawGame(Game& game, sf::RenderWindow& window);
    void ShutdownGame(Game& game);

    bool IsEnableOptions(const Game& game, GameOptions option);
}
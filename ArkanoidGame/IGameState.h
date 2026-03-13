#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace ArkanoidGame
{
    // Base interface for all game states. Provides a virtual interface for state management.
    class IGameState
    {
    public:
        virtual ~IGameState() = default;

        // Called when state is pushed onto the stack (enters active state)
        virtual void onEnter() {}

        // Called when state is popped from the stack (leaves active state)
        virtual void onExit() {}

        // Handle window events (keyboard, mouse, etc.)
        virtual void handleEvent(const sf::Event& event) = 0;

        // Update game logic
        virtual void update(float timeDelta) = 0;

        // Render state to window
        virtual void draw(sf::RenderWindow& window) = 0;

        // Optional: request state change
        // Returns nullptr to continue in current state, or a new state to transition to
        virtual std::unique_ptr<IGameState> getNextState() { return nullptr; }

        // Get state name for debugging and comparisons
        virtual const char* getStateName() const { return "UnknownState"; }
    };

    using GameStatePtr = std::unique_ptr<IGameState>;
}

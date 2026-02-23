#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"

namespace ArkanoidGame {

    class Application
    {
    public:
        Application(const Application& app) = delete;
        Application& operator= (const Application&) = delete;

        static Application& Instance();

        void Run();
        sf::RenderWindow& GetWindow() { return window; }
        Game& GetGame() { return game; }

    private:
        Application();
        ~Application();

    private:
        Game game;
        sf::RenderWindow window;
    };

}
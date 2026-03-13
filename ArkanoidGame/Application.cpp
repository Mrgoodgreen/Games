#include "Application.h"
#include "GameSettings.h"
#include <cstdlib>
#include <iostream>
#include <ctime>

namespace ArkanoidGame
{
    // Define PLAYER_NAME in one translation unit
    const char* PLAYER_NAME = "Player";

    Application& Application::Instance()
    {
        std::cout << "[Application] Getting instance..." << std::endl;
        static Application instance;
        std::cout << "[Application] Instance ready" << std::endl;
        return instance;
    }

    Application::Application() :
        window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_NAME)
    {
        std::cout << "[Application] Constructor: window created" << std::endl;
        
        unsigned int seed = (unsigned int)time(nullptr);
        srand(seed);

        std::cout << "[Application] Constructor: calling InitGame..." << std::endl;
        InitGame(game);
        std::cout << "[Application] Constructor: InitGame complete" << std::endl;
    }

    Application::~Application()
    {
        std::cout << "[Application] Destructor: shutting down game..." << std::endl;
        ShutdownGame(game);
        std::cout << "[Application] Destructor: shutdown complete" << std::endl;
    }

    void Application::Run()
    {
        std::cout << "[Application] Run() started" << std::endl;
        sf::Clock gameClock;

        // If InitGame pushed a state without calling onEnter (deferred), activate it now.
        if (game.hasActiveState()) {
            IGameState* initial = game.getCurrentState();
            if (initial) {
                std::cout << "[Application] Activating initial state..." << std::endl;
                initial->onEnter();
            }
        }

        int frameCount = 0;
        while (window.isOpen()) {
            frameCount++;
            if (frameCount % 60 == 0) {
                std::cout << "[Application] Frame: " << frameCount << ", States: " << game.stateStack.size() << std::endl;
            }

            float startTime = gameClock.getElapsedTime().asSeconds();

            HandleWindowEvents(game, window);

            if (!window.isOpen()) {
                std::cout << "[Application] Window closed by event" << std::endl;
                break;
            }

            if (UpdateGame(game, TIME_PER_FRAME))
            {
                window.clear();

                DrawGame(game, window);

                window.display();
            }
            else
            {
                std::cout << "[Application] UpdateGame returned false - closing window" << std::endl;
                window.close();
            }

            float endTime = gameClock.getElapsedTime().asSeconds();
            float deltaTime = endTime - startTime;
            if (deltaTime < TIME_PER_FRAME) {
                sf::sleep(sf::seconds(TIME_PER_FRAME - deltaTime));
            }
        }
        std::cout << "[Application] Run() finished" << std::endl;
    }
}
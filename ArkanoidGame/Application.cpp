#include "Application.h"
#include "GameSettings.h"
#include <cstdlib>

namespace ArkanoidGame
{
    // Define PLAYER_NAME in one translation unit
    const char* PLAYER_NAME = "Player";

    Application& Application::Instance()
    {
        static Application instance;
        return instance;
    }

    Application::Application() :
        window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_NAME)
    {
        unsigned int seed = (unsigned int)time(nullptr);
        srand(seed);

        InitGame(game);
    }

    Application::~Application()
    {
        ShutdownGame(game);
    }

    void Application::Run()
    {
        sf::Clock gameClock;

        while (window.isOpen()) {

            float startTime = gameClock.getElapsedTime().asSeconds();

            HandleWindowEvents(game, window);

            if (!window.isOpen()) {
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
                window.close();
            }

            float endTime = gameClock.getElapsedTime().asSeconds();
            float deltaTime = endTime - startTime;
            if (deltaTime < TIME_PER_FRAME) {
                sf::sleep(sf::seconds(TIME_PER_FRAME - deltaTime));
            }
        }
    }
}
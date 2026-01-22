#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace ApplesGame;

int main()
{
    // Create window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEGHT), "Apples Game");
    window.setFramerateLimit(60);

    GameState gameState;
    bool modeChosen = false;

    // Mode selection UI - allocate on heap to avoid stack overflow (C6262)
    sf::Font* font = new sf::Font();
    font->loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf");

    sf::Text* modeHint = new sf::Text();
    modeHint->setFont(*font);
    modeHint->setCharacterSize(18);
    modeHint->setFillColor(sf::Color::White);
    modeHint->setString(
        "=== GAME MODE SELECTION ===\n\n"
        "PRESET MODES:\n"
        "1 - 20 apples + acceleration (infinite)\n"
        "2 - 20 apples, no acceleration (infinite)\n"
        "3 - 50 apples + acceleration (infinite)\n"
        "4 - 20 apples + acceleration (finite)\n"
        "5 - 50 apples, no acceleration (finite)\n\n"
        "CUSTOM MODE (combine flags):\n"
        "6 - Select individual flags\n\n"
        "Press 1-6...\n"
        "ESC to exit"
    );
    modeHint->setPosition(40.f, 40.f);

    // Mode selection screen loop
    while (window.isOpen() && !modeChosen)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                delete modeHint;
                delete font;
                return 0;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                // PRESET MODE 1: 20 apples, acceleration, infinite
                if (event.key.code == sf::Keyboard::Num1)
                {
                    gameState.gameModeMask = MODE_APPLES_INFINITE | MODE_SPEED_ACCEL_ON;
                    gameState.applesDefaultCount = 20;
                    gameState.applesCount = 20;
                    modeChosen = true;
                }
                // PRESET MODE 2: 20 apples, no acceleration, infinite
                else if (event.key.code == sf::Keyboard::Num2)
                {
                    gameState.gameModeMask = MODE_APPLES_INFINITE | MODE_SPEED_ACCEL_OFF;
                    gameState.applesDefaultCount = 20;
                    gameState.applesCount = 20;
                    modeChosen = true;
                }
                // PRESET MODE 3: 50 apples, acceleration, infinite
                else if (event.key.code == sf::Keyboard::Num3)
                {
                    gameState.gameModeMask = MODE_APPLES_INFINITE | MODE_SPEED_ACCEL_ON;
                    gameState.applesDefaultCount = 50;
                    gameState.applesCount = 50;
                    modeChosen = true;
                }
                // PRESET MODE 4: 20 apples, acceleration, finite
                else if (event.key.code == sf::Keyboard::Num4)
                {
                    gameState.gameModeMask = MODE_APPLES_FINITE | MODE_SPEED_ACCEL_ON;
                    gameState.applesDefaultCount = 20;
                    gameState.applesCount = 20;
                    modeChosen = true;
                }
                // PRESET MODE 5: 50 apples, no acceleration, finite
                else if (event.key.code == sf::Keyboard::Num5)
                {
                    gameState.gameModeMask = MODE_APPLES_FINITE | MODE_SPEED_ACCEL_OFF;
                    gameState.applesDefaultCount = 50;
                    gameState.applesCount = 50;
                    modeChosen = true;
                }
                // CUSTOM MODE 6: Select individual flags
                else if (event.key.code == sf::Keyboard::Num6)
                {
                    // Show custom mode selection - allocate on heap
                    bool customModeChosen = false;
                    gameState.gameModeMask = 0;
                    int applesCount = 20;

                    sf::Text* customModeText = new sf::Text();
                    customModeText->setFont(*font);
                    customModeText->setCharacterSize(16);
                    customModeText->setFillColor(sf::Color::White);

                    // Step 1: Select apple count
                    bool step1Done = false;
                    while (window.isOpen() && !step1Done)
                    {
                        customModeText->setString(
                            "=== CUSTOM MODE ===\n\n"
                            "Step 1: Select apple count\n"
                            "1 - 20 apples\n"
                            "2 - 50 apples\n"
                            "3 - Random count (15-50)\n"
                            "ESC to return to main menu"
                        );
                        customModeText->setPosition(40.f, 40.f);

                        while (window.pollEvent(event))
                        {
                            if (event.type == sf::Event::Closed)
                            {
                                window.close();
                                delete customModeText;
                                delete modeHint;
                                delete font;
                                CleanupGame(gameState);
                                return 0;
                            }
                            if (event.type == sf::Event::KeyPressed)
                            {
                                if (event.key.code == sf::Keyboard::Num1)
                                {
                                    applesCount = 20;
                                    step1Done = true;
                                }
                                else if (event.key.code == sf::Keyboard::Num2)
                                {
                                    applesCount = 50;
                                    step1Done = true;
                                }
                                else if (event.key.code == sf::Keyboard::Num3)
                                {
                                    applesCount = 15 + (std::rand() % 36); // Random 15-50
                                    step1Done = true;
                                }
                                else if (event.key.code == sf::Keyboard::Escape)
                                {
                                    step1Done = true;
                                    customModeChosen = true; // Return to main menu
                                }
                            }
                        }

                        window.clear(sf::Color::Black);
                        window.draw(*customModeText);
                        window.display();
                    }

                    if (!customModeChosen)
                    {
                        gameState.applesDefaultCount = applesCount;
                        gameState.applesCount = applesCount;

                        // Step 2: Select apple respawn mode
                        bool step2Done = false;
                        while (window.isOpen() && !step2Done)
                        {
                            customModeText->setString(
                                "=== CUSTOM MODE ===\n\n"
                                "Step 2: Select apple respawn mode\n"
                                "1 - Infinite (apples respawn)\n"
                                "2 - Finite (apples disappear)\n"
                                "ESC to return to apple count"
                            );
                            customModeText->setPosition(40.f, 40.f);

                            while (window.pollEvent(event))
                            {
                                if (event.type == sf::Event::Closed)
                                {
                                    window.close();
                                    delete customModeText;
                                    delete modeHint;
                                    delete font;
                                    CleanupGame(gameState);
                                    return 0;
                                }
                                if (event.type == sf::Event::KeyPressed)
                                {
                                    if (event.key.code == sf::Keyboard::Num1)
                                    {
                                        gameState.gameModeMask |= MODE_APPLES_INFINITE;
                                        step2Done = true;
                                    }
                                    else if (event.key.code == sf::Keyboard::Num2)
                                    {
                                        gameState.gameModeMask |= MODE_APPLES_FINITE;
                                        step2Done = true;
                                    }
                                    else if (event.key.code == sf::Keyboard::Escape)
                                    {
                                        step2Done = true;
                                    }
                                }
                            }

                            window.clear(sf::Color::Black);
                            window.draw(*customModeText);
                            window.display();
                        }

                        if (gameState.gameModeMask != 0)
                        {
                            // Step 3: Select acceleration mode
                            bool step3Done = false;
                            while (window.isOpen() && !step3Done)
                            {
                                customModeText->setString(
                                    "=== CUSTOM MODE ===\n\n"
                                    "Step 3: Select acceleration mode\n"
                                    "1 - Acceleration ON\n"
                                    "2 - Acceleration OFF\n"
                                    "ESC to return to respawn mode"
                                );
                                customModeText->setPosition(40.f, 40.f);

                                while (window.pollEvent(event))
                                {
                                    if (event.type == sf::Event::Closed)
                                    {
                                        window.close();
                                        delete customModeText;
                                        delete modeHint;
                                        delete font;
                                        CleanupGame(gameState);
                                        return 0;
                                    }
                                    if (event.type == sf::Event::KeyPressed)
                                    {
                                        if (event.key.code == sf::Keyboard::Num1)
                                        {
                                            gameState.gameModeMask |= MODE_SPEED_ACCEL_ON;
                                            modeChosen = true;
                                            step3Done = true;
                                        }
                                        else if (event.key.code == sf::Keyboard::Num2)
                                        {
                                            gameState.gameModeMask |= MODE_SPEED_ACCEL_OFF;
                                            modeChosen = true;
                                            step3Done = true;
                                        }
                                        else if (event.key.code == sf::Keyboard::Escape)
                                        {
                                            step3Done = true;
                                        }
                                    }
                                }

                                window.clear(sf::Color::Black);
                                window.draw(*customModeText);
                                window.display();
                            }
                        }
                    }

                    delete customModeText;
                }
                // Exit
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    delete modeHint;
                    delete font;
                    return 0;
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(*modeHint);
        window.display();
    }

    if (!window.isOpen())
    {
        delete modeHint;
        delete font;
        return 0;
    }

    // Initialize game after mode selection
    InitGame(gameState);

    // Main game loop
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }

        float timeDelta = clock.restart().asSeconds();
        HandleInput(gameState);
        UpdateGame(gameState, timeDelta);

        window.clear(sf::Color::Black);
        DrawGame(gameState, window);
        window.display();
    }

    // Cleanup - deallocate all resources
    delete modeHint;
    delete font;
    CleanupGame(gameState);

    return 0;
}
#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace ApplesGame;

int main()
{
    // окно
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEGHT), "Apples Game");
    window.setFramerateLimit(60);

    GameState gameState;

    // --- выбор режима до InitGame ---
    bool modeChosen = false;

    // Подсказка по режимам
    sf::Font font;
    font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf");

    sf::Text modeHint;
    modeHint.setFont(font);
    modeHint.setCharacterSize(22);
    modeHint.setFillColor(sf::Color::White);
    modeHint.setString(
        "Choose mode:\n"
        "1 - 20 apples, accel (infinite)\n"
        "2 - 20 apples, no accel (infinite)\n"
        "3 - 50 apples, accel (infinite)\n"
        "4 - 20 apples, finite + accel\n"
        "5 - 50 apples, finite, no accel\n"
        "Press 1-5..."
    );
    modeHint.setPosition(40.f, 40.f);

    while (window.isOpen() && !modeChosen)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                // 1: 20 яблок, ускорение
                if (event.key.code == sf::Keyboard::Num1)
                {
                    gameState.gameModeMask = MODE_APPLES_INFINITE | MODE_SPEED_ACCEL_ON;
                    gameState.applesCount = 20;
                    gameState.applesTargetTotal = -1;
                    modeChosen = true;
                }
                // 2: 20 яблок, без ускорения
                else if (event.key.code == sf::Keyboard::Num2)
                {
                    gameState.gameModeMask = MODE_APPLES_INFINITE | MODE_SPEED_ACCEL_OFF;
                    gameState.applesCount = 20;
                    gameState.applesTargetTotal = -1;
                    modeChosen = true;
                }
                // 3: 50 яблок, ускорение
                else if (event.key.code == sf::Keyboard::Num3)
                {
                    gameState.gameModeMask = MODE_APPLES_INFINITE | MODE_SPEED_ACCEL_ON;
                    gameState.applesCount = 50;
                    gameState.applesTargetTotal = -1;
                    modeChosen = true;
                }
                // 4: конечные 20, с ускорением
                else if (event.key.code == sf::Keyboard::Num4)
                {
                    gameState.gameModeMask = MODE_APPLES_FINITE | MODE_SPEED_ACCEL_ON;
                    gameState.applesCount = 20;
                    gameState.applesTargetTotal = 20;
                    modeChosen = true;
                }
                // 5: конечные 50, без ускорения
                else if (event.key.code == sf::Keyboard::Num5)
                {
                    gameState.gameModeMask = MODE_APPLES_FINITE | MODE_SPEED_ACCEL_OFF;
                    gameState.applesCount = 50;
                    gameState.applesTargetTotal = 50;
                    modeChosen = true;
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(modeHint);
        window.display();
    }

    if (!window.isOpen())
        return 0;

    // инициализация игры после выбора режима
    InitGame(gameState);

    // основной цикл
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

    // очистка динамического массива
    delete[] gameState.apples;
    gameState.apples = nullptr;

    return 0;
}

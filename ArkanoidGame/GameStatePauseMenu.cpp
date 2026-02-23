#include "GameStatePauseMenu.h"
#include "Application.h"
#include "Game.h"
#include <assert.h>

namespace ArkanoidGame
{
    void GameStatePauseMenuData::init()
    {
        assert(font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

        background.setFillColor(sf::Color(0, 0, 0, 128)); // semi-transparent black

        titleText.setString("Pause");
        titleText.setFont(font);
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::Red);

        MenuItem resumeItem;
        resumeItem.text.setString("Return to game");
        resumeItem.text.setFont(font);
        resumeItem.text.setCharacterSize(24);
        resumeItem.onPressCallback = [](MenuItem&) {
            PopGameState(Application::Instance().GetGame());
            };

        MenuItem exitItem;
        exitItem.text.setString("Exit to main menu");
        exitItem.text.setFont(font);
        exitItem.text.setCharacterSize(24);
        exitItem.onPressCallback = [](MenuItem&) {
            SwitchGameState(Application::Instance().GetGame(), GameStateType::MainMenu);
            };

        MenuItem pauseMenu;
        pauseMenu.childrenOrientation = Orientation::Vertical;
        pauseMenu.childrenAlignment = Alignment::Middle;
        pauseMenu.childrens.push_back(resumeItem);
        pauseMenu.childrens.push_back(exitItem);

        menu.Init(pauseMenu);
    }

    void GameStatePauseMenuData::shutdown()
    {
        // resources are released automatically
    }

    void GameStatePauseMenuData::handleWindowEvent(const sf::Event& event)
    {
        Game& game = Application::Instance().GetGame();
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                PopGameState(game);
            }

            if (event.key.code == sf::Keyboard::Enter)
            {
                menu.PressOnSelectedItem();
            }

            if (event.key.code == sf::Keyboard::Up)
            {
                menu.SwitchToPreviousMenuItem();
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                menu.SwitchToNextMenuItem();
            }
        }
    }

    void GameStatePauseMenuData::update(float /*timeDelta*/)
    {
    }

    void GameStatePauseMenuData::draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

        background.setSize(viewSize);
        window.draw(background);

        titleText.setOrigin(GetTextOrigin(titleText, { 0.5f, 0.f }));
        titleText.setPosition(viewSize.x / 2.f, 100);
        window.draw(titleText);

        menu.Draw(window, window.getView().getCenter(), { 0.5f, 0.f });
    }

    // Free-function wrappers
    void InitGameStatePauseMenu(GameStatePauseMenuData& data) { data.init(); }
    void ShutdownGameStatePauseMenu(GameStatePauseMenuData& data) { data.shutdown(); }
    void HandleGameStatePauseMenuWindowEvent(GameStatePauseMenuData& data, const sf::Event& event) { data.handleWindowEvent(event); }
    void UpdateGameStatePauseMenu(GameStatePauseMenuData& data, float timeDelta) { data.update(timeDelta); }
    void DrawGameStatePauseMenu(GameStatePauseMenuData& data, sf::RenderWindow& window) { data.draw(window); }
}
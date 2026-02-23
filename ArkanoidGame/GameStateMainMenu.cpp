#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include <assert.h>

namespace ArkanoidGame
{
    void GameStateMainMenuData::init()
    {
        assert(font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

        MenuItem startGame;
        startGame.text.setString("Start Game");
        startGame.text.setFont(font);
        startGame.text.setCharacterSize(24);
        startGame.onPressCallback = [](MenuItem&) {
            SwitchGameState(Application::Instance().GetGame(), GameStateType::Playing);
            };

        bool isInfiniteLives = IsEnableOptions(Application::Instance().GetGame(), GameOptions::InfiniteLives);
        MenuItem optionsInfiniteLivesItem;
        optionsInfiniteLivesItem.text.setString("Infinite Lives: " + std::string(isInfiniteLives ? "On" : "Off"));
        optionsInfiniteLivesItem.text.setFont(font);
        optionsInfiniteLivesItem.text.setCharacterSize(24);
        optionsInfiniteLivesItem.onPressCallback = [](MenuItem& item) {
            Game& game = Application::Instance().GetGame();
            game.setOptions((GameOptions)((std::uint8_t)game.getOptions() ^ (std::uint8_t)GameOptions::InfiniteLives));
            bool isInfiniteLives = IsEnableOptions(game, GameOptions::InfiniteLives);
            item.text.setString("Infinite Lives: " + std::string(isInfiniteLives ? "On" : "Off"));
            };

        bool isUnlimitedBalls = IsEnableOptions(Application::Instance().GetGame(), GameOptions::UnlimitedBalls);
        MenuItem optionsUnlimitedBallsItem;
        optionsUnlimitedBallsItem.text.setString("Unlimited Balls: " + std::string(isUnlimitedBalls ? "On" : "Off"));
        optionsUnlimitedBallsItem.text.setFont(font);
        optionsUnlimitedBallsItem.text.setCharacterSize(24);
        optionsUnlimitedBallsItem.onPressCallback = [](MenuItem& item) {
            Game& game = Application::Instance().GetGame();
            game.setOptions((GameOptions)((std::uint8_t)game.getOptions() ^ (std::uint8_t)GameOptions::UnlimitedBalls));
            bool isUnlimitedBalls = IsEnableOptions(game, GameOptions::UnlimitedBalls);
            item.text.setString("Unlimited Balls: " + std::string(isUnlimitedBalls ? "On" : "Off"));
            };

        MenuItem options;
        options.text.setString("Options");
        options.text.setFont(font);
        options.text.setCharacterSize(24);
        options.hintText.setString("Options");
        options.hintText.setFont(font);
        options.hintText.setCharacterSize(48);
        options.hintText.setFillColor(sf::Color::Red);
        options.childrenOrientation = Orientation::Vertical;
        options.childrenAlignment = Alignment::Middle;
        options.childrenSpacing = 10.f;
        options.childrens.push_back(optionsInfiniteLivesItem);
        options.childrens.push_back(optionsUnlimitedBallsItem);

        MenuItem recordsItem;
        recordsItem.text.setString("Records");
        recordsItem.text.setFont(font);
        recordsItem.text.setCharacterSize(24);
        recordsItem.onPressCallback = [](MenuItem&) {
            PushGameState(Application::Instance().GetGame(), GameStateType::Records, true);
            };

        MenuItem yesItem;
        yesItem.text.setString("Yes");
        yesItem.text.setFont(font);
        yesItem.text.setCharacterSize(24);
        yesItem.onPressCallback = [](MenuItem&) {
            SwitchGameState(Application::Instance().GetGame(), GameStateType::None);
            };

        MenuItem noItem;
        noItem.text.setString("No");
        noItem.text.setFont(font);
        noItem.text.setCharacterSize(24);
        noItem.onPressCallback = [this](MenuItem&) {
            menu.GoBack();
            };

        MenuItem exitGameItem;
        exitGameItem.text.setString("Exit Game");
        exitGameItem.text.setFont(font);
        exitGameItem.text.setCharacterSize(24);
        exitGameItem.hintText.setString("Are you sure?");
        exitGameItem.hintText.setFont(font);
        exitGameItem.hintText.setCharacterSize(48);
        exitGameItem.hintText.setFillColor(sf::Color::Red);
        exitGameItem.childrenOrientation = Orientation::Horizontal;
        exitGameItem.childrenAlignment = Alignment::Middle;
        exitGameItem.childrenSpacing = 10.f;
        exitGameItem.childrens.push_back(yesItem);
        exitGameItem.childrens.push_back(noItem);

        MenuItem mainMenu;
        mainMenu.hintText.setString("Arkanoid Game");
        mainMenu.hintText.setFont(font);
        mainMenu.hintText.setCharacterSize(48);
        mainMenu.hintText.setFillColor(sf::Color::Red);
        mainMenu.childrenOrientation = Orientation::Vertical;
        mainMenu.childrenAlignment = Alignment::Middle;
        mainMenu.childrenSpacing = 10.f;
        mainMenu.childrens.push_back(startGame);
        mainMenu.childrens.push_back(options);
        mainMenu.childrens.push_back(recordsItem);
        mainMenu.childrens.push_back(exitGameItem);

        menu.Init(mainMenu);
    }

    void GameStateMainMenuData::shutdown()
    {
        // resources freed automatically
    }

    void GameStateMainMenuData::handleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                menu.GoBack();
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                menu.PressOnSelectedItem();
            }

            Orientation orientation = menu.GetCurrentContext().childrenOrientation;
            if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Up ||
                orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Left)
            {
                menu.SwitchToPreviousMenuItem();
            }
            else if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Down ||
                orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Right)
            {
                menu.SwitchToNextMenuItem();
            }
        }
    }

    void GameStateMainMenuData::update(float /*timeDelta*/)
    {

    }

    void GameStateMainMenuData::draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

        sf::Text* hintText = &menu.GetCurrentContext().hintText;
        hintText->setOrigin(GetTextOrigin(*hintText, { 0.5f, 0.f }));
        hintText->setPosition(viewSize.x / 2.f, 150.f);
        window.draw(*hintText);

        menu.Draw(window, viewSize / 2.f, { 0.5f, 0.f });
    }

    // Free function wrappers
    void InitGameStateMainMenu(GameStateMainMenuData& data) { data.init(); }
    void ShutdownGameStateMainMenu(GameStateMainMenuData& data) { data.shutdown(); }
    void HandleGameStateMainMenuWindowEvent(GameStateMainMenuData& data, const sf::Event& event) { data.handleWindowEvent(event); }
    void UpdateGameStateMainMenu(GameStateMainMenuData& data, float timeDelta) { data.update(timeDelta); }
    void DrawGameStateMainMenu(GameStateMainMenuData& data, sf::RenderWindow& window) { data.draw(window); }
}
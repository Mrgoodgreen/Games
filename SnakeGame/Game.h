#pragma once
#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Apple.h"
#include "GameField.h"
#include "Menu.h"
#include "Settings.h"
#include "ScoreTable.h"
#include "Utils.h"

class Game
{
public:
    Game();
    ~Game();

    void Run();

private:
    void Initialize();
    void ProcessEvents();
    void Update(float deltaTime);
    void Render();
    
    void HandleMainMenuInput(sf::Keyboard::Key key);
    void HandleDifficultyMenuInput(sf::Keyboard::Key key);
    void HandleSettingsMenuInput(sf::Keyboard::Key key);
    void HandleScoreTableInput(sf::Keyboard::Key key);
    void HandleGameInput(sf::Keyboard::Key key);
    void HandlePauseInput(sf::Keyboard::Key key);
    void HandleGameOverInput(sf::Keyboard::Key key);
    void HandleNameInputInput(sf::Keyboard::Key key);
    
    void StartGame();
    void ResetGame();
    void GameOver();
    void SpawnApple();
    void UpdateGame(float deltaTime);
    
    sf::RenderWindow m_Window;
    sf::Clock m_Clock;
    
    Snake m_Snake;
    Apple m_Apple;
    GameField m_GameField;
    Menu m_Menu;
    Settings m_Settings;
    ScoreTable m_ScoreTable;
    
    Utils::GameState m_CurrentState;
    int m_CurrentScore;
    int m_CurrentDifficulty;
    int m_MenuSelection;
    int m_SettingsSelection;
    int m_PauseSelection;
    int m_GameOverSelection;
    
    float m_CountdownTimer;
    bool m_IsCountingDown;
    
    bool m_WaitingForNameInput;
    bool m_NameInputYesSelected;
    std::string m_PlayerName;
};

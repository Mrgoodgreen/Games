#include "Game.h"
#include "GameConfig.h"
#include <cstdlib>
#include <ctime>

Game::Game()
    : m_Window(sf::VideoMode(GameConfig::WindowWidth, GameConfig::WindowHeight), GameConfig::WindowTitle)
    , m_CurrentState(Utils::GameState::MainMenu)
    , m_CurrentScore(0)
    , m_CurrentDifficulty(0)
    , m_MenuSelection(0)
    , m_SettingsSelection(0)
    , m_PauseSelection(0)
    , m_GameOverSelection(0)
    , m_CountdownTimer(0.0f)
    , m_IsCountingDown(false)
    , m_WaitingForNameInput(false)
    , m_NameInputYesSelected(false)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Initialize();
}

Game::~Game()
{
}

void Game::Initialize()
{
    m_Snake.LoadTextures();
    m_Apple.LoadTexture();
    m_Menu.LoadFont();
    m_Settings.Initialize();
    m_ScoreTable.LoadFont();
    m_ScoreTable.LoadScores();
}

void Game::Run()
{
    m_Clock.restart();
    
    while (m_Window.isOpen())
    {
        float deltaTime = m_Clock.restart().asSeconds();
        
        ProcessEvents();
        Update(deltaTime);
        Render();
    }
}

void Game::ProcessEvents()
{
    sf::Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_Window.close();
        }
        
        if (event.type == sf::Event::KeyPressed)
        {
            if (m_WaitingForNameInput)
            {
                HandleNameInputInput(event.key.code);
            }
            else
            {
                switch (m_CurrentState)
                {
                    case Utils::GameState::MainMenu:
                        HandleMainMenuInput(event.key.code);
                        break;
                    case Utils::GameState::DifficultySelection:
                        HandleDifficultyMenuInput(event.key.code);
                        break;
                    case Utils::GameState::SettingsMenu:
                        HandleSettingsMenuInput(event.key.code);
                        break;
                    case Utils::GameState::ScoreTable:
                        HandleScoreTableInput(event.key.code);
                        break;
                    case Utils::GameState::Playing:
                        HandleGameInput(event.key.code);
                        break;
                    case Utils::GameState::Paused:
                        HandlePauseInput(event.key.code);
                        break;
                    case Utils::GameState::GameOver:
                        HandleGameOverInput(event.key.code);
                        break;
                }
            }
        }
        
        if (event.type == sf::Event::TextEntered && m_WaitingForNameInput)
        {
            if (event.text.unicode >= 32 && event.text.unicode < 128)
            {
                if (m_PlayerName.length() < GameConfig::MaxNameLength)
                {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    if ((enteredChar >= 'a' && enteredChar <= 'z') || 
                        (enteredChar >= 'A' && enteredChar <= 'Z'))
                    {
                        m_PlayerName += enteredChar;
                    }
                }
            }
            else if (event.text.unicode == 8 && !m_PlayerName.empty())
            {
                m_PlayerName.pop_back();
            }
        }
    }
}

void Game::HandleMainMenuInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::W)
    {
        m_Settings.PlayButtonClick();
        m_MenuSelection = (m_MenuSelection - 1 + 5) % 5;
    }
    else if (key == sf::Keyboard::S)
    {
        m_Settings.PlayButtonClick();
        m_MenuSelection = (m_MenuSelection + 1) % 5;
    }
    else if (key == sf::Keyboard::Enter)
    {
        m_Settings.PlayButtonClick();
        
        switch (m_MenuSelection)
        {
            case 0:
                StartGame();
                break;
            case 1:
                m_CurrentState = Utils::GameState::DifficultySelection;
                break;
            case 2:
                m_CurrentState = Utils::GameState::ScoreTable;
                break;
            case 3:
                m_CurrentState = Utils::GameState::SettingsMenu;
                m_SettingsSelection = 0;
                break;
            case 4:
                m_Window.close();
                break;
        }
    }
}

void Game::HandleDifficultyMenuInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::W)
    {
        m_Settings.PlayButtonClick();
        m_CurrentDifficulty = (m_CurrentDifficulty - 1 + 5) % 5;
    }
    else if (key == sf::Keyboard::S)
    {
        m_Settings.PlayButtonClick();
        m_CurrentDifficulty = (m_CurrentDifficulty + 1) % 5;
    }
    else if (key == sf::Keyboard::Enter)
    {
        m_Settings.PlayButtonClick();
    }
    else if (key == sf::Keyboard::B)
    {
        m_Settings.PlayButtonClick();
        m_CurrentState = Utils::GameState::MainMenu;
    }
}

void Game::HandleSettingsMenuInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::W)
    {
        m_Settings.PlayButtonClick();
        m_SettingsSelection = (m_SettingsSelection - 1 + 3) % 3;
    }
    else if (key == sf::Keyboard::S)
    {
        m_Settings.PlayButtonClick();
        m_SettingsSelection = (m_SettingsSelection + 1) % 3;
    }
    else if (key == sf::Keyboard::Enter)
    {
        m_Settings.PlayButtonClick();
        
        if (m_SettingsSelection == 0)
        {
            m_Settings.SetSoundEnabled(!m_Settings.IsSoundEnabled());
        }
        else if (m_SettingsSelection == 1)
        {
            m_Settings.SetMusicEnabled(!m_Settings.IsMusicEnabled());
        }
        else if (m_SettingsSelection == 2)
        {
            m_CurrentState = Utils::GameState::MainMenu;
        }
    }
    else if (key == sf::Keyboard::B)
    {
        m_Settings.PlayButtonClick();
        m_CurrentState = Utils::GameState::MainMenu;
    }
}

void Game::HandleScoreTableInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::B)
    {
        m_Settings.PlayButtonClick();
        m_CurrentState = Utils::GameState::MainMenu;
    }
}

void Game::HandleGameInput(sf::Keyboard::Key key)
{
    if (m_IsCountingDown)
    {
        return;
    }
    
    if (key == sf::Keyboard::W)
    {
        m_Snake.SetDirection(Utils::Direction::Up);
    }
    else if (key == sf::Keyboard::S)
    {
        m_Snake.SetDirection(Utils::Direction::Down);
    }
    else if (key == sf::Keyboard::A)
    {
        m_Snake.SetDirection(Utils::Direction::Left);
    }
    else if (key == sf::Keyboard::D)
    {
        m_Snake.SetDirection(Utils::Direction::Right);
    }
    else if (key == sf::Keyboard::P)
    {
        m_Settings.PlayButtonClick();
        m_CurrentState = Utils::GameState::Paused;
        m_PauseSelection = 0;
        m_Settings.PauseMusic();
    }
}

void Game::HandlePauseInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::W || key == sf::Keyboard::S)
    {
        m_Settings.PlayButtonClick();
        m_PauseSelection = (m_PauseSelection + 1) % 2;
    }
    else if (key == sf::Keyboard::Enter)
    {
        m_Settings.PlayButtonClick();
        
        if (m_PauseSelection == 0)
        {
            m_CurrentState = Utils::GameState::Playing;
            m_IsCountingDown = true;
            m_CountdownTimer = GameConfig::PauseDelay;
            m_Settings.PlayGameStart();
        }
        else
        {
            m_Settings.StopMusic();
            m_CurrentState = Utils::GameState::MainMenu;
            m_MenuSelection = 0;
        }
    }
}

void Game::HandleGameOverInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::W || key == sf::Keyboard::S)
    {
        m_Settings.PlayButtonClick();
        m_GameOverSelection = (m_GameOverSelection + 1) % 2;
    }
    else if (key == sf::Keyboard::Enter)
    {
        m_Settings.PlayButtonClick();
        
        if (m_GameOverSelection == 0)
        {
            StartGame();
        }
        else
        {
            m_CurrentState = Utils::GameState::MainMenu;
            m_MenuSelection = 0;
        }
    }
}

void Game::HandleNameInputInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::A || key == sf::Keyboard::D)
    {
        m_Settings.PlayButtonClick();
        m_NameInputYesSelected = !m_NameInputYesSelected;
    }
    else if (key == sf::Keyboard::Enter)
    {
        m_Settings.PlayButtonClick();
        
        if (m_NameInputYesSelected)
        {
            std::string finalName = m_PlayerName.empty() ? "XYZ" : m_PlayerName;
            m_ScoreTable.AddScore(finalName, m_CurrentScore);
        }
        
        m_WaitingForNameInput = false;
        m_PlayerName.clear();
    }
}

void Game::Update(float deltaTime)
{
    if (m_CurrentState == Utils::GameState::Playing)
    {
        if (m_IsCountingDown)
        {
            m_CountdownTimer -= deltaTime;
            if (m_CountdownTimer <= 0.0f)
            {
                m_IsCountingDown = false;
                m_Settings.StartMusic();
            }
        }
        else
        {
            UpdateGame(deltaTime);
        }
    }
}

void Game::UpdateGame(float deltaTime)
{
    float currentSpeed = GameConfig::DifficultySpeed[m_CurrentDifficulty];
    m_Snake.Update(deltaTime, currentSpeed);
    
    sf::Vector2i headPos = m_Snake.GetHeadPosition();
    
    if (m_GameField.IsWallCollision(headPos) || m_Snake.CheckSelfCollision())
    {
        GameOver();
        return;
    }
    
    if (headPos == m_Apple.GetPosition())
    {
        m_Settings.PlayEatApple();
        m_CurrentScore += GameConfig::DifficultyPoints[m_CurrentDifficulty];
        m_Snake.Grow(GameConfig::SnakeGrowthAmount);
        SpawnApple();
    }
}

void Game::Render()
{
    m_Window.clear(GameConfig::BackgroundColor);
    
    switch (m_CurrentState)
    {
        case Utils::GameState::MainMenu:
            m_Menu.DrawMainMenu(m_Window, m_MenuSelection);
            break;
            
        case Utils::GameState::DifficultySelection:
            m_Menu.DrawDifficultyMenu(m_Window, m_CurrentDifficulty);
            break;
            
        case Utils::GameState::SettingsMenu:
            m_Menu.DrawSettingsMenu(
                m_Window, 
                m_SettingsSelection, 
                m_Settings.IsSoundEnabled(),
                m_Settings.IsMusicEnabled()
            );
            break;
            
        case Utils::GameState::ScoreTable:
            m_ScoreTable.DrawFullTable(m_Window);
            break;
            
        case Utils::GameState::Playing:
            m_GameField.Draw(m_Window);
            m_Snake.Draw(m_Window);
            m_Apple.Draw(m_Window);
            m_ScoreTable.DrawInGame(m_Window, m_CurrentScore);
            
            if (m_IsCountingDown)
            {
                m_Menu.DrawCountdown(m_Window, m_CountdownTimer);
            }
            break;
            
        case Utils::GameState::Paused:
            m_GameField.Draw(m_Window);
            m_Snake.Draw(m_Window);
            m_Apple.Draw(m_Window);
            m_ScoreTable.DrawInGame(m_Window, m_CurrentScore);
            m_Menu.DrawPauseMenu(m_Window, m_PauseSelection);
            break;
            
        case Utils::GameState::GameOver:
            m_GameField.Draw(m_Window);
            
            if (m_WaitingForNameInput)
            {
                m_ScoreTable.DrawNameInput(m_Window, m_PlayerName, m_NameInputYesSelected);
            }
            else
            {
                m_ScoreTable.DrawPopup(m_Window, m_CurrentScore, m_GameOverSelection);
            }
            break;
    }
    
    m_Window.display();
}

void Game::StartGame()
{
    ResetGame();
    m_CurrentState = Utils::GameState::Playing;
    m_IsCountingDown = true;
    m_CountdownTimer = GameConfig::StartDelay;
    m_Settings.PlayGameStart();
}

void Game::ResetGame()
{
    m_CurrentScore = 0;
    
    sf::Vector2i startPos(
        GameConfig::FieldWidth / 2,
        GameConfig::FieldHeight / 2
    );
    
    m_Snake.Initialize(startPos, GameConfig::InitialSnakeLength);
    SpawnApple();
}

void Game::GameOver()
{
    m_Settings.StopMusic();
    m_Settings.PlayGameOver();
    m_Settings.PlayCollision();
    
    m_CurrentState = Utils::GameState::GameOver;
    m_GameOverSelection = 0;
    
    if (m_ScoreTable.IsHighScore(m_CurrentScore))
    {
        m_WaitingForNameInput = true;
        m_NameInputYesSelected = false;
        m_PlayerName.clear();
    }
    else
    {
        m_WaitingForNameInput = false;
    }
}

void Game::SpawnApple()
{
    sf::Vector2i newPosition;
    bool validPosition = false;
    
    int maxAttempts = 1000;
    int attempts = 0;
    
    while (!validPosition && attempts < maxAttempts)
    {
        newPosition.x = 1 + std::rand() % (GameConfig::FieldWidth - 2);
        newPosition.y = 1 + std::rand() % (GameConfig::FieldHeight - 2);
        
        if (!m_Snake.IsPositionOnSnake(newPosition))
        {
            validPosition = true;
        }
        
        attempts++;
    }
    
    m_Apple.SetPosition(newPosition);
}

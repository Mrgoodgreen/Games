#include "ScoreTable.h"
#include "GameConfig.h"
#include <fstream>
#include <algorithm>
#include <cassert>

ScoreTable::ScoreTable()
{
}

ScoreTable::~ScoreTable()
{
}

void ScoreTable::LoadScores()
{
    m_Scores.clear();
    
    std::ifstream file(GameConfig::ScoreFilePath, std::ios::binary);
    if (file.is_open())
    {
        int count = 0;
        file.read(reinterpret_cast<char*>(&count), sizeof(int));
        
        for (int i = 0; i < count && i < GameConfig::MaxScoresInTable; ++i)
        {
            ScoreEntry entry;
            file.read(reinterpret_cast<char*>(&entry), sizeof(ScoreEntry));
            m_Scores.push_back(entry);
        }
        
        file.close();
    }
}

void ScoreTable::SaveScores()
{
    std::ofstream file(GameConfig::ScoreFilePath, std::ios::binary);
    if (file.is_open())
    {
        int count = static_cast<int>(m_Scores.size());
        file.write(reinterpret_cast<const char*>(&count), sizeof(int));
        
        for (const auto& entry : m_Scores)
        {
            file.write(reinterpret_cast<const char*>(&entry), sizeof(ScoreEntry));
        }
        
        file.close();
    }
}

void ScoreTable::AddScore(const std::string& playerName, int score)
{
    m_Scores.push_back(ScoreEntry(playerName, score));
    SortScores();
    
    if (m_Scores.size() > GameConfig::MaxScoresInTable)
    {
        m_Scores.resize(GameConfig::MaxScoresInTable);
    }
    
    SaveScores();
}

bool ScoreTable::IsHighScore(int score) const
{
    if (m_Scores.size() < GameConfig::MaxScoresInTable)
    {
        return true;
    }
    
    return score > m_Scores.back().Score;
}

void ScoreTable::DrawInGame(sf::RenderWindow& window, int currentScore)
{
    sf::Text scoreText;
    scoreText.setFont(m_Font);
    scoreText.setString("Score: " + std::to_string(currentScore));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(GameConfig::ScoreColor);
    scoreText.setPosition(
        GameConfig::WindowWidth - 150.0f,
        20.0f
    );
    
    window.draw(scoreText);
}

void ScoreTable::DrawPopup(sf::RenderWindow& window, int currentScore, int selectedOption)
{
    sf::RectangleShape overlay(sf::Vector2f(
        static_cast<float>(GameConfig::WindowWidth),
        static_cast<float>(GameConfig::WindowHeight)
    ));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::RectangleShape popup(sf::Vector2f(400.0f, 450.0f));
    popup.setPosition(200.0f, 75.0f);
    popup.setFillColor(sf::Color(50, 50, 50));
    popup.setOutlineColor(sf::Color::White);
    popup.setOutlineThickness(2.0f);
    window.draw(popup);

    sf::Text gameOverText;
    gameOverText.setFont(m_Font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(270.0f, 100.0f);
    window.draw(gameOverText);

    sf::Text scoreText;
    scoreText.setFont(m_Font);
    scoreText.setString("Your Score: " + std::to_string(currentScore));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(GameConfig::ScoreColor);
    scoreText.setPosition(280.0f, 150.0f);
    window.draw(scoreText);

    sf::Text highScoresTitle;
    highScoresTitle.setFont(m_Font);
    highScoresTitle.setString("High Scores:");
    highScoresTitle.setCharacterSize(20);
    highScoresTitle.setFillColor(sf::Color::White);
    highScoresTitle.setPosition(310.0f, 190.0f);
    window.draw(highScoresTitle);

    int displayCount = std::min(GameConfig::MaxScoresInPopup, static_cast<int>(m_Scores.size()));
    for (int i = 0; i < displayCount; ++i)
    {
        sf::Text entryText;
        entryText.setFont(m_Font);
        entryText.setString(
            std::to_string(i + 1) + ". " + 
            std::string(m_Scores[i].Name) + " - " + 
            std::to_string(m_Scores[i].Score)
        );
        entryText.setCharacterSize(18);
        entryText.setFillColor(sf::Color::White);
        entryText.setPosition(240.0f, 230.0f + i * 30.0f);
        window.draw(entryText);
    }

    sf::Text startGameText;
    startGameText.setFont(m_Font);
    startGameText.setString("Start Game");
    startGameText.setCharacterSize(22);
    startGameText.setFillColor(selectedOption == 0 ? GameConfig::SelectedColor : GameConfig::TextColor);
    startGameText.setPosition(310.0f, 420.0f);
    window.draw(startGameText);

    sf::Text exitText;
    exitText.setFont(m_Font);
    exitText.setString("Exit to Menu");
    exitText.setCharacterSize(22);
    exitText.setFillColor(selectedOption == 1 ? GameConfig::SelectedColor : GameConfig::TextColor);
    exitText.setPosition(300.0f, 460.0f);
    window.draw(exitText);
}

void ScoreTable::DrawFullTable(sf::RenderWindow& window)
{
    sf::Text titleText;
    titleText.setFont(m_Font);
    titleText.setString("HIGH SCORES");
    titleText.setCharacterSize(36);
    titleText.setFillColor(GameConfig::ScoreColor);
    titleText.setPosition(280.0f, 50.0f);
    window.draw(titleText);

    int displayCount = std::min(GameConfig::MaxScoresInTable, static_cast<int>(m_Scores.size()));
    for (int i = 0; i < displayCount; ++i)
    {
        sf::Text entryText;
        entryText.setFont(m_Font);
        entryText.setString(
            std::to_string(i + 1) + ". " + 
            std::string(m_Scores[i].Name) + " - " + 
            std::to_string(m_Scores[i].Score)
        );
        entryText.setCharacterSize(24);
        entryText.setFillColor(sf::Color::White);
        entryText.setPosition(250.0f, 130.0f + i * 40.0f);
        window.draw(entryText);
    }

    sf::Text backText;
    backText.setFont(m_Font);
    backText.setString("Press B to go back");
    backText.setCharacterSize(18);
    backText.setFillColor(GameConfig::TextColor);
    backText.setPosition(290.0f, 550.0f);
    window.draw(backText);
}

void ScoreTable::DrawNameInput(sf::RenderWindow& window, const std::string& currentName, bool yesSelected)
{
    sf::RectangleShape overlay(sf::Vector2f(
        static_cast<float>(GameConfig::WindowWidth),
        static_cast<float>(GameConfig::WindowHeight)
    ));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::RectangleShape popup(sf::Vector2f(400.0f, 250.0f));
    popup.setPosition(200.0f, 175.0f);
    popup.setFillColor(sf::Color(50, 50, 50));
    popup.setOutlineColor(sf::Color::White);
    popup.setOutlineThickness(2.0f);
    window.draw(popup);

    sf::Text questionText;
    questionText.setFont(m_Font);
    questionText.setString("New High Score!");
    questionText.setCharacterSize(28);
    questionText.setFillColor(GameConfig::ScoreColor);
    questionText.setPosition(250.0f, 200.0f);
    window.draw(questionText);

    sf::Text promptText;
    promptText.setFont(m_Font);
    promptText.setString("Enter your name?");
    promptText.setCharacterSize(22);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(270.0f, 245.0f);
    window.draw(promptText);

    sf::Text nameText;
    nameText.setFont(m_Font);
    nameText.setString(currentName.empty() ? "XYZ" : currentName);
    nameText.setCharacterSize(24);
    nameText.setFillColor(GameConfig::ScoreColor);
    nameText.setPosition(330.0f, 290.0f);
    window.draw(nameText);

    sf::Text yesText;
    yesText.setFont(m_Font);
    yesText.setString("Yes");
    yesText.setCharacterSize(22);
    yesText.setFillColor(yesSelected ? GameConfig::SelectedColor : GameConfig::TextColor);
    yesText.setPosition(300.0f, 350.0f);
    window.draw(yesText);

    sf::Text noText;
    noText.setFont(m_Font);
    noText.setString("No");
    noText.setCharacterSize(22);
    noText.setFillColor(!yesSelected ? GameConfig::SelectedColor : GameConfig::TextColor);
    noText.setPosition(450.0f, 350.0f);
    window.draw(noText);
}

const std::vector<ScoreEntry>& ScoreTable::GetScores() const
{
    return m_Scores;
}

bool ScoreTable::LoadFont()
{
    bool loaded = m_Font.loadFromFile(GameConfig::FontPath);
    assert(loaded && "Failed to load font");
    return loaded;
}

void ScoreTable::SortScores()
{
    std::sort(m_Scores.begin(), m_Scores.end(), 
        [](const ScoreEntry& a, const ScoreEntry& b)
        {
            return a.Score > b.Score;
        });
}

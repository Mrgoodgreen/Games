#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct ScoreEntry
{
    char Name[21];
    int Score;

    ScoreEntry() : Score(0)
    {
        Name[0] = '\0';
    }

    ScoreEntry(const std::string& name, int score) : Score(score)
    {
        size_t length = name.length();
        if (length > 20)
        {
            length = 20;
        }
        
        for (size_t i = 0; i < length; ++i)
        {
            Name[i] = name[i];
        }
        Name[length] = '\0';
    }
};

class ScoreTable
{
public:
    ScoreTable();
    ~ScoreTable();

    void LoadScores();
    void SaveScores();
    void AddScore(const std::string& playerName, int score);
    bool IsHighScore(int score) const;
    
    void DrawInGame(sf::RenderWindow& window, int currentScore);
    void DrawPopup(sf::RenderWindow& window, int currentScore, int selectedOption);
    void DrawFullTable(sf::RenderWindow& window);
    void DrawNameInput(sf::RenderWindow& window, const std::string& currentName, bool yesSelected);

    const std::vector<ScoreEntry>& GetScores() const;

    bool LoadFont();

private:
    std::vector<ScoreEntry> m_Scores;
    sf::Font m_Font;
    
    void SortScores();
};

#pragma once
#include <SFML/Audio.hpp>

class Settings
{
public:
    Settings();
    ~Settings();

    void Initialize();
    
    bool IsSoundEnabled() const;
    bool IsMusicEnabled() const;
    
    void SetSoundEnabled(bool enabled);
    void SetMusicEnabled(bool enabled);
    
    void PlayButtonClick();
    void PlayCollision();
    void PlayGameStart();
    void PlayGameOver();
    void PlayEatApple();
    
    void StartMusic();
    void StopMusic();
    void PauseMusic();
    
    bool LoadSounds();

private:
    bool m_SoundEnabled;
    bool m_MusicEnabled;
    
    sf::SoundBuffer m_ButtonClickBuffer;
    sf::SoundBuffer m_CollisionBuffer;
    sf::SoundBuffer m_GameStartBuffer;
    sf::SoundBuffer m_GameOverBuffer;
    sf::SoundBuffer m_EatAppleBuffer;
    
    sf::Sound m_ButtonClickSound;
    sf::Sound m_CollisionSound;
    sf::Sound m_GameStartSound;
    sf::Sound m_GameOverSound;
    sf::Sound m_EatAppleSound;
    
    sf::Music m_GameMusic;
};

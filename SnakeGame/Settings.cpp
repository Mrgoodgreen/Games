#include "Settings.h"
#include "GameConfig.h"
#include <cassert>

Settings::Settings()
    : m_SoundEnabled(true)
    , m_MusicEnabled(true)
{
}

Settings::~Settings()
{
    StopMusic();
}

void Settings::Initialize()
{
    LoadSounds();
}

bool Settings::IsSoundEnabled() const
{
    return m_SoundEnabled;
}

bool Settings::IsMusicEnabled() const
{
    return m_MusicEnabled;
}

void Settings::SetSoundEnabled(bool enabled)
{
    m_SoundEnabled = enabled;
}

void Settings::SetMusicEnabled(bool enabled)
{
    m_MusicEnabled = enabled;
    
    if (!enabled)
    {
        StopMusic();
    }
}

void Settings::PlayButtonClick()
{
    if (m_SoundEnabled)
    {
        m_ButtonClickSound.play();
    }
}

void Settings::PlayCollision()
{
    if (m_SoundEnabled)
    {
        m_CollisionSound.play();
    }
}

void Settings::PlayGameStart()
{
    if (m_SoundEnabled)
    {
        m_GameStartSound.play();
    }
}

void Settings::PlayGameOver()
{
    if (m_SoundEnabled)
    {
        m_GameOverSound.play();
    }
}

void Settings::PlayEatApple()
{
    if (m_SoundEnabled)
    {
        m_EatAppleSound.play();
    }
}

void Settings::StartMusic()
{
    if (m_MusicEnabled)
    {
        m_GameMusic.setLoop(true);
        m_GameMusic.play();
    }
}

void Settings::StopMusic()
{
    m_GameMusic.stop();
}

void Settings::PauseMusic()
{
    m_GameMusic.pause();
}

bool Settings::LoadSounds()
{
    bool allLoaded = true;
    
    bool buttonClickLoaded = m_ButtonClickBuffer.loadFromFile(GameConfig::ButtonClickSoundPath);
    assert(buttonClickLoaded && "Failed to load button click sound");
    allLoaded = allLoaded && buttonClickLoaded;
    
    bool collisionLoaded = m_CollisionBuffer.loadFromFile(GameConfig::CollisionSoundPath);
    assert(collisionLoaded && "Failed to load collision sound");
    allLoaded = allLoaded && collisionLoaded;
    
    bool gameStartLoaded = m_GameStartBuffer.loadFromFile(GameConfig::GameStartSoundPath);
    assert(gameStartLoaded && "Failed to load game start sound");
    allLoaded = allLoaded && gameStartLoaded;
    
    bool gameOverLoaded = m_GameOverBuffer.loadFromFile(GameConfig::GameOverSoundPath);
    assert(gameOverLoaded && "Failed to load game over sound");
    allLoaded = allLoaded && gameOverLoaded;
    
    bool eatAppleLoaded = m_EatAppleBuffer.loadFromFile(GameConfig::EatAppleSoundPath);
    assert(eatAppleLoaded && "Failed to load eat apple sound");
    allLoaded = allLoaded && eatAppleLoaded;
    
    bool musicLoaded = m_GameMusic.openFromFile(GameConfig::GameMusicPath);
    assert(musicLoaded && "Failed to load game music");
    allLoaded = allLoaded && musicLoaded;
    
    if (allLoaded)
    {
        m_ButtonClickSound.setBuffer(m_ButtonClickBuffer);
        m_CollisionSound.setBuffer(m_CollisionBuffer);
        m_GameStartSound.setBuffer(m_GameStartBuffer);
        m_GameOverSound.setBuffer(m_GameOverBuffer);
        m_EatAppleSound.setBuffer(m_EatAppleBuffer);
    }
    
    return allLoaded;
}

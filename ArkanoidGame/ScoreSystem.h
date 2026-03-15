#pragma once
#include <vector>
#include <algorithm>

namespace ArkanoidGame
{
    class IScoreObserver
    {
    public:
        virtual ~IScoreObserver() = default;
        virtual void OnScoreChanged(int newScore) = 0;
    };

    class ScoreSystem
    {
    public:
        void AddObserver(IScoreObserver* observer)
        {
            if (!observer)
                return;

            if (std::find(m_Observers.begin(), m_Observers.end(), observer) == m_Observers.end())
            {
                m_Observers.push_back(observer);
            }
        }

        void RemoveObserver(IScoreObserver* observer)
        {
            m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
        }

        void Reset()
        {
            m_Score = 0;
            Notify();
        }

        void AddPoints(int points)
        {
            m_Score += points;
            Notify();
        }

        int GetScore() const { return m_Score; }

    private:
        void Notify()
        {
            for (IScoreObserver* observer : m_Observers)
            {
                observer->OnScoreChanged(m_Score);
            }
        }

        int m_Score = 0;
        std::vector<IScoreObserver*> m_Observers;
    };
}

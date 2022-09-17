#pragma once
#include "DisplayText.h"
#include "Constants.h"

class Score final
{
private:
    uint32_t m_Score = 0;
    DisplayText m_Text;
private:
    inline void UpdateText()
    {
        m_Text.Update(m_Score);
        m_Text.CenterX(Const::WindowWidth);
    }
public:
    inline Score() : m_Text(m_Score, Const::ScoreFontSize, DARKGREEN)
    {
        m_Text.CenterX(Const::WindowWidth);
    }


    inline Score& operator=(uint32_t newScore)
    {
        m_Score = newScore;
        UpdateText();
        return *this;
    }


    inline Score& operator++() 
    {
        ++m_Score;
        UpdateText();
        return *this;
    }


    inline void Draw() const { m_Text.Draw(); }
};
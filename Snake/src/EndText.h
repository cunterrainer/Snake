#pragma once

#include "DisplayText.h"
#include "Constants.h"

class EndText
{
private:
    DisplayText m_Text{ "Failed!", Const::DoneFontSize, RED };
private:
    inline void UpdateText(const std::string_view& str, const Color& color)
    {
        m_Text.Update(str);
        m_Text.Update(color);
        m_Text.Center(Const::WindowWidth, Const::WindowHeight);
    }
public:
    inline EndText()
    {
        m_Text.Center(Const::WindowWidth, Const::WindowHeight);
    }


    inline void SetWin() { UpdateText("Won!", GOLD); }
    inline bool Reset(float dt)  
    { 
        static float alpha = 255;
        static float diff = 0.f;
        float d = 5.f;
        diff += 500 * dt;

        if (alpha <= 0.f) // faded
        {
            alpha = 255.f+d;
            diff = 0.f;
            Color c = m_Text.GetColor();
            c.a = 255;
            UpdateText(m_Text.GetText(), c);
            return true;
        }
        if(diff >= d)
        {
            alpha -= diff;
            diff = 0.f;
            Color c = m_Text.GetColor();
            c.a = alpha <= 0.f ? 0 : static_cast<unsigned char>(alpha);
            UpdateText(m_Text.GetText(), c);
        }
        return false;
    }


    inline void Draw(bool done) const 
    { 
        if(done)
            m_Text.Draw();
    }
};
#pragma once

#include "DisplayText.h"
#include "Constants.h"

class EndText final
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
        static float alpha = 255.f;
        static float diff = 0.f;
        static float yOffset = 0.f;
        diff += Const::EndTextFadingDiffMP * dt;

        bool faded = false;
        if(diff >= Const::EndTextFadingStep)
        {
            alpha -= diff;
            faded = alpha <= 0.f;
            Color c = m_Text.GetColor();

            if (faded)
            {
                yOffset = 0.f;
                alpha = 255.f;
            }
            else
                yOffset += diff / Const::EndTextYOffsetDenom;

            diff = 0.f;
            c.a = static_cast<unsigned char>(alpha);
            UpdateText(m_Text.GetText(), c);
            m_Text.SetY(m_Text.GetY() - yOffset);
        }
        return faded;
    }


    inline void Draw(bool done) const 
    { 
        if(done)
            m_Text.Draw();
    }
};
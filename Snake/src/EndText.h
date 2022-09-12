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
    inline void Reset()  { UpdateText("Failed!", RED); }

    inline void Draw(bool done) const 
    { 
        if(done)
            m_Text.Draw();
    }
};
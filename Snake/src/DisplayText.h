#pragma once
#include <string>
#include <string_view>
#include <cstdint>

#include "raylib.h"

class DisplayText
{
private:
    std::string m_Text;
    int m_XPos = 0;
    int m_YPos = 0;
    int m_FontSize;
    Color m_Color;
public:
    DisplayText(const std::string& text, int fontSize, const Color& color) : m_Text(text), m_FontSize(fontSize), m_Color(color) {}
    DisplayText(uint32_t value, int fontSize, const Color& color) : DisplayText(std::to_string(value), fontSize, color) {}

    inline void CenterX(uint16_t windowWidth)
    {
        const int textLength = MeasureText(m_Text.c_str(), m_FontSize);
        m_XPos = static_cast<int>(static_cast<float>(windowWidth) / 2.f - static_cast<float>(textLength) / 2.f);
    }


    inline void CenterY(uint16_t windowHeight)
    {
        m_YPos = static_cast<int>(static_cast<float>(windowHeight) / 2.f - static_cast<float>(m_FontSize) / 2.f);
    }


    inline void Center(uint16_t windowWidth, uint16_t windowHeight) { CenterX(windowWidth); CenterY(windowHeight); }
    inline void Update(const std::string_view& str) { m_Text = str; }
    inline void Update(uint32_t val) { Update(std::to_string(val)); }
    inline void Update(const Color& color) { m_Color = color; }
    inline const std::string& GetText() const { return m_Text; }
    inline const Color& GetColor() const { return m_Color; }


    inline void Draw() const
    {
        DrawText(m_Text.c_str(), m_XPos, m_YPos, m_FontSize, m_Color);
    }
};
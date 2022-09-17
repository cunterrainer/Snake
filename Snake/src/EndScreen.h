#pragma once
#include "raylib.h"

#include "DisplayText.h"
#include "Constants.h"
#include "Layer.h"

class EndScreen final : public Layer
{
private:
    DisplayText m_Text{ "Failed!", Const::DoneFontSize, RED };
    LayerStage m_Done = LayerStage::NotDone;
    bool m_Fade = false;
    bool m_Win = false;
private:
    inline void UpdateText(const std::string_view& str, const Color& color)
    {
        m_Text.Update(str);
        m_Text.Update(color);
        m_Text.Center(Const::WindowWidth, Const::WindowHeight);
    }


    inline bool Reset(float dt)
    {
        static float alpha = 255.f;
        static float diff = 0.f;
        static float yOffset = 0.f;
        diff += Const::EndTextFadingDiffMP * dt;

        bool faded = false;
        if (diff >= Const::EndTextFadingStep)
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
public:
    inline EndScreen()
    {
        m_Text.Center(Const::WindowWidth, Const::WindowHeight);
    }


    inline void SetWin(bool win) override
    { 
        if (win)
        {
            UpdateText("Won!", GOLD);
            m_Win = win;
        }
    }


    inline void OnKeyPress(int keyPressed, float dt) override
    {
        if (m_Fade || keyPressed == KEY_SPACE)
        {
            m_Fade = !Reset(dt);
            if (!m_Fade)
                m_Done = LayerStage::DoneEndScreen;
        }
    }


    inline void OnUpdate(float) override 
    { 
        /*Gets updated in OnKeyPress*/ 
    }


    inline void OnRender() const override
    { 
        if(m_Done == LayerStage::NotDone)
            m_Text.Draw();
    }


    inline bool Win() const override
    {
        return m_Win;
    }


    inline LayerStage Done() override
    {
        const LayerStage returnStage = m_Done == LayerStage::DoneEndScreen ? LayerStage::DoneEndScreen : LayerStage::NotDone;
        m_Done = LayerStage::NotDone;
        return returnStage;
    }
};
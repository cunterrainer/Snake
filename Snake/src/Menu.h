#pragma once
#include "raylib.h"

#include "Layer.h"
#include "DisplayText.h"
#include "Constants.h"

class Menu final : public Layer
{
private:
    DisplayText m_Text{"W|A|S|D", Const::DoneFontSize, RAYWHITE};
    LayerStage m_Stage = LayerStage::NotDone;
public:
    inline Menu()
    {
        m_Text.Center(Const::WindowWidth, Const::WindowHeight);
    }


    inline void SetWin(bool) override
    {
        /*Will always be false since it's being called before the game layer*/
    }


    inline void OnKeyPress(int keyPressed, float) override
    {
        switch (keyPressed)
        {
        case KEY_W:
        case KEY_UP:
        case KEY_A:
        case KEY_LEFT:
        case KEY_S:
        case KEY_DOWN:
        case KEY_D:
        case KEY_RIGHT:
        case KEY_SPACE:
        case KEY_ENTER:
            break;
        default:
            return;
        }
        m_Stage = LayerStage::Done;
    }


    inline void OnUpdate(float) override
    {
        /*
            Gets updated in OnKeyPress()
        */
    }


    inline void OnRender() const override
    {
        m_Text.Draw();
    }


    inline bool Win() const override
    { 
        return false; 
    }


    inline LayerStage Done() override
    {
        return m_Stage;
    }
};
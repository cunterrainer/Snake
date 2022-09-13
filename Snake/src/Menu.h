#pragma once

#include "Layer.h"
#include "DisplayText.h"
#include "Constants.h"
#include "Game.h"

class Menu final : public Layer
{
private:
    DisplayText m_Text{"W|A|S|D", Const::DoneFontSize, RAYWHITE};
public:
    Menu()
    {
        m_Text.Center(Const::WindowWidth, Const::WindowHeight);
    }

    void OnKeyPress(int keyPressed, float) override
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
            break;
        default:
            return;
        }
        GameSetStarted() = true;
    }


    void OnUpdate(float) override 
    {
        /*
            Gets updated in OnKeyPress()
        */
    }


    void OnRender() const override
    {
        if(!GameHasStarted())
            m_Text.Draw();
    }
};
#pragma once

#include "Layer.h"
#include "DisplayText.h"

class Menu final : public Layer
{
private:
    DisplayText m_Text{"Menu", Const::DoneFontSize, RAYWHITE};
public:
    void OnKeyPress([[maybe_unused]] int keyPressed, [[maybe_unused]] float dt)
    {

    }


    void OnUpdate([[maybe_unused]]float dt)
    {

    }


    void OnRender() const
    {
        m_Text.Draw();
    }
};
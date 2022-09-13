#pragma once

#include "Layer.h"
#include "DisplayText.h"

class Menu final : public Layer
{
private:
    DisplayText m_Text{"Menu", Const::DoneFontSize, RAYWHITE};
public:
    void OnKeyPress([[maybe_unused]] int keyPressed, [[maybe_unused]] float dt) override
    {

    }


    void OnUpdate([[maybe_unused]]float dt) override
    {

    }


    void OnRender() const override
    {
        m_Text.Draw();
    }
};
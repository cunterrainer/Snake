#pragma once

struct Layer
{
    virtual ~Layer() = default;

    virtual void OnKeyPress(int keyPressed, float dt) = 0;
    virtual void OnUpdate(float dt) = 0;
    virtual void OnRender() const = 0;
};
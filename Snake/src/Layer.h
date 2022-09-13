#pragma once

struct Layer
{
    virtual ~Layer() = default;

    virtual void OnKeyPress(int, float) = 0;
    virtual void OnUpdate(float) = 0;
    virtual void OnRender() const = 0;
};
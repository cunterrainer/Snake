#pragma once

enum class LayerStage
{
    NotDone, Done, DoneEndScreen
};

struct Layer
{
    virtual ~Layer() = default;

    virtual void SetWin(bool) = 0;
    virtual void OnKeyPress(int, float) = 0;
    virtual void OnUpdate(float) = 0;
    virtual void OnRender() const = 0;
    virtual bool Win() const = 0;
    virtual LayerStage Done() = 0;
};
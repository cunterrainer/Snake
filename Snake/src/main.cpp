#include <memory>
#include <vector>
#include <utility>

#include "raylib.h"

#include "Constants.h"
#include "Clang.h"
#include "Layer.h"
#include "Menu.h"
#include "Game.h"
#include "EndScreen.h"
#include "PauseMenu.h"


template <class T> constexpr void PushBackLayer(std::vector<std::unique_ptr<Layer>>& layers)
{
    layers.push_back(std::make_unique<T>());
}


int main()
{
    static constexpr Color gridColor = { 255, 255, 255, 75 };
    static constexpr Color backgroundColor{ 10, 10, 10, 255 };

    InitWindow(Const::WindowWidth, Const::WindowHeight, Const::WindowTitle);
    SetTargetFPS(Const::TargetFPS);
    SetExitKey(KEY_NULL);

    std::vector<std::unique_ptr<Layer>> layers;
    PushBackLayer<Menu>(layers);
    PushBackLayer<Game>(layers);
    PushBackLayer<EndScreen>(layers);
    std::unique_ptr<Layer> pauseLayer = std::make_unique<PauseMenu>();

    std::vector<std::unique_ptr<Layer>>::iterator layerIt = layers.begin();
    Layer* currentLayer = layerIt->get();
    bool win = currentLayer->Win();

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        BeginDrawing();
        ClearBackground(backgroundColor);
        for (const Rectangle& cell : Const::Grid)
            DrawRectangleLinesEx(cell, Const::GridOutlineThickness, gridColor);

        int keyPressed = GetKeyPressed();
        if (keyPressed == KEY_ESCAPE && currentLayer != pauseLayer.get())
        {
            currentLayer = pauseLayer.get();
            keyPressed = KEY_NULL;
        }

        currentLayer->SetWin(win);
        currentLayer->OnKeyPress(keyPressed, deltaTime);
        currentLayer->OnUpdate(deltaTime);
        currentLayer->OnRender();
        win = currentLayer->Win();

        const LayerStage stage = currentLayer->Done();
        if (stage == LayerStage::Done)
        {
            layerIt = std::next(layerIt);
            currentLayer = layerIt->get();
        }
        else if (stage == LayerStage::DoneEndScreen) 
        {
            layerIt = std::prev(layerIt);
            currentLayer = layerIt->get();
        }
        else if (stage == LayerStage::DonePause)
        {
            currentLayer = layerIt->get();
        }
        EndDrawing();
    }
    TerminateWindow();
}
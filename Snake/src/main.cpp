#include <array>
#include <memory>
#include <vector>
#include <utility>

#include "raylib.h"

#include "Constants.h"
#include "Clang.h"
#include "Layer.h"
#include "Menu.h"
#include "Game.h"


template <class T> constexpr void PushBackLayer(std::vector<std::unique_ptr<Layer>>& layers)
{
    layers.push_back(std::make_unique<T>());
}


template <class Func, class... Args>
void ForEachLayer(const std::vector<std::unique_ptr<Layer>>& layers, const Func& func, Args&&... args)
{
    for (const std::unique_ptr<Layer>& layer : layers)
        (layer.get()->*func)(std::forward<Args>(args)...);
}


int main()
{
    static constexpr std::array<Rectangle, Const::GridSize> grid = GenerateGrid();
    static constexpr Color gridColor = { 255, 255, 255, 75 };
    static constexpr Color backgroundColor{ 10, 10, 10, 255 };

    InitWindow(Const::WindowWidth, Const::WindowHeight, Const::WindowTitle);
    SetTargetFPS(Const::TargetFPS);
    SetExitKey(KEY_NULL);

    std::vector<std::unique_ptr<Layer>> layers;
    PushBackLayer<Menu>(layers);
    PushBackLayer<Game>(layers); // Game first since the menu is rendered on top of the grid
    PushBackLayer<EndScreen>(layers);

    std::vector<std::unique_ptr<Layer>>::iterator layerIt = layers.begin();
    Layer* currentLayer = layerIt->get();
    bool win = currentLayer->Win();

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        BeginDrawing();
        ClearBackground(backgroundColor);
        for (const Rectangle& cell : grid)
            DrawRectangleLinesEx(cell, Const::GridOutlineThickness, gridColor);

        currentLayer->SetWin(win);
        currentLayer->OnKeyPress(GetKeyPressed(), deltaTime);
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

        EndDrawing();
    }
    TerminateWindow();
}
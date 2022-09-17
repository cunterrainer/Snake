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


static constexpr std::array<Rectangle, Const::GridSize> GenerateGrid()
{
    std::array<Rectangle, Const::GridSize> grid{};
    uint16_t currentX = 0;
    uint16_t currentY = 0;

    for (size_t i = 0; i < grid.size(); ++i)
    {
        grid[i].x = currentX;
        grid[i].y = currentY;
        grid[i].width = Const::CellSize;
        grid[i].height = Const::CellSize;

        const bool endOfLine = (i + 1) % Const::BoardWidth == 0;
        currentX = endOfLine ? 0 : static_cast<uint16_t>(currentX + Const::CellSize);
        currentY = endOfLine ? static_cast<uint16_t>(currentY + Const::CellSize) : currentY;
    }
    return grid;
}


template <class T> constexpr void PushBackLayer(std::vector<std::unique_ptr<Layer>>& layers)
{
    layers.push_back(std::make_unique<T>());
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
        for (const Rectangle& cell : grid)
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
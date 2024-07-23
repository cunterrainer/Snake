#include <memory>
#include <vector>
#include <iterator>

#include "raylib.h"

#include "Constants.h"
#include "Clang.h"
#include "Layer.h"
#include "Menu.h"
#include "Game.h"
#include "EndScreen.h"
#include "PauseMenu.h"


#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>

struct GameObjects
{
    Color backgroundColor;
    Color gridColor;

    Layer* currentLayer;
    std::vector<std::unique_ptr<Layer>>::iterator layerIt;
    bool win;
    Layer* pauseLayer;
};


void GameLoop(void* args)
{
    GameObjects* obj = (GameObjects*)args;

    const float deltaTime = GetFrameTime();
    BeginDrawing();
    ClearBackground(obj->backgroundColor);
    for (const Rectangle& cell : Const::Grid)
        DrawRectangleLinesEx(cell, Const::GridOutlineThickness, obj->gridColor);

    int keyPressed = GetKeyPressed();
    if (IsGamepadAvailable(0))
    {
        if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
            keyPressed = KEY_W;
        else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
            keyPressed = KEY_D;
        else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
            keyPressed = KEY_S;
        else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
            keyPressed = KEY_A;
        else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
            keyPressed = KEY_SPACE;
        else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT))
            keyPressed = KEY_ESCAPE;
        else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP))
            keyPressed = KEY_R;

    }
    if (keyPressed == KEY_ESCAPE && obj->currentLayer != obj->pauseLayer)
    {
        obj->currentLayer = obj->pauseLayer;
        keyPressed = KEY_NULL;
    }


    obj->currentLayer->SetWin(obj->win);
    obj->currentLayer->OnKeyPress(keyPressed, deltaTime);
    obj->currentLayer->OnUpdate(deltaTime);
    obj->currentLayer->OnRender();
    obj->win = obj->currentLayer->Win();

    const LayerStage stage = obj->currentLayer->Done();
    if (stage == LayerStage::Done)
    {
        obj->layerIt = std::next(obj->layerIt);
        obj->currentLayer = obj->layerIt->get();
    }
    else if (stage == LayerStage::DoneEndScreen)
    {
        obj->layerIt = std::prev(obj->layerIt);
        obj->currentLayer = obj->layerIt->get();
    }
    else if (stage == LayerStage::DonePause)
    {
        obj->currentLayer = obj->layerIt->get();
    }
    EndDrawing();
}
#endif // PLATFORM_WEB


template <class T> constexpr void PushBackLayer(std::vector<std::unique_ptr<Layer>>& layers)
{
    layers.push_back(std::make_unique<T>());
}


int main()
{
    static constexpr Color gridColor = { 255, 255, 255, 75 };
    static constexpr Color backgroundColor{ 10, 10, 10, 255 };

    InitWindow(Const::WindowWidth, Const::WindowHeight, Const::WindowTitle);
    SetExitKey(KEY_NULL);

    std::vector<std::unique_ptr<Layer>> layers;
    PushBackLayer<Menu>(layers);
    PushBackLayer<Game>(layers);
    PushBackLayer<EndScreen>(layers);
    std::unique_ptr<Layer> pauseLayer = std::make_unique<PauseMenu>();

    std::vector<std::unique_ptr<Layer>>::iterator layerIt = layers.begin();
    Layer* currentLayer = layerIt->get();
    bool win = currentLayer->Win();

#ifdef PLATFORM_WEB
    GameObjects objects;
    objects.backgroundColor = backgroundColor;
    objects.gridColor = gridColor;
    objects.win = win;
    objects.currentLayer = currentLayer;
    objects.layerIt = layerIt;
    objects.pauseLayer = pauseLayer.get();
    emscripten_set_main_loop_arg(GameLoop, &objects, 0, 1);
#else
    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        BeginDrawing();
        ClearBackground(backgroundColor);
        for (const Rectangle& cell : Const::Grid)
            DrawRectangleLinesEx(cell, Const::GridOutlineThickness, gridColor);

        int keyPressed = GetKeyPressed();
        if (IsGamepadAvailable(0))
        {
            if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
                keyPressed = KEY_W;
            else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
                keyPressed = KEY_D;
            else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
                keyPressed = KEY_S;
            else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
                keyPressed = KEY_A;
            else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
                keyPressed = KEY_SPACE;
            else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT))
                keyPressed = KEY_ESCAPE;
            else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP))
                keyPressed = KEY_R;

        }
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
#endif
    TerminateWindow();
}
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
constexpr void ForEachLayer(const std::vector<std::unique_ptr<Layer>>& layers, const Func& func, Args&&... args)
{
    for (const std::unique_ptr<Layer>& layer : layers)
        (layer.get()->*func)(std::forward<Args>(args)...);
}


int main()
{
    static constexpr Color backgroundColor{ 10, 10, 10, 255 };
    InitWindow(Const::WindowWidth, Const::WindowHeight, Const::WindowTitle);
    SetTargetFPS(Const::TargetFPS);
    SetExitKey(KEY_NULL);

    std::vector<std::unique_ptr<Layer>> layers;
    PushBackLayer<Game>(layers); // Game first since the menu is rendered on top of the grid
    PushBackLayer<Menu>(layers);

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        ForEachLayer(layers, &Layer::OnKeyPress, GetKeyPressed(), deltaTime);
        ForEachLayer(layers, &Layer::OnUpdate, deltaTime);

        BeginDrawing();
        ClearBackground(backgroundColor);
        ForEachLayer(layers, &Layer::OnRender);
        EndDrawing();
    }
    TerminateWindow();
}
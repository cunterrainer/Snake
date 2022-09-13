#include <memory>

#include "raylib.h"

#include "Constants.h"
#include "Clang.h"
#include "Game.h"


int main()
{
    static constexpr Color backgroundColor{ 10, 10, 10, 255 };
    InitWindow(Const::WindowWidth, Const::WindowHeight, Const::WindowTitle);
    SetTargetFPS(Const::TargetFPS);
    SetExitKey(KEY_NULL);

    std::unique_ptr<Game> game = std::make_unique<Game>();
    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        game->OnKeyPress(GetKeyPressed(), deltaTime);
        game->OnUpdate(deltaTime);

        BeginDrawing();
        ClearBackground(backgroundColor);
        game->OnRender();
        EndDrawing();
    }

    TerminateWindow();
}
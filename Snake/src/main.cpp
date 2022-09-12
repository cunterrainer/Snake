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


//int main()
//{
//    static constexpr std::array<Rectangle, Const::GridSize> grid = GenerateGrid();
//    static constexpr Color backgroundColor{ 10, 10, 10, 255 };
//    InitWindow(Const::WindowWidth, Const::WindowHeight, Const::WindowTitle);
//    SetTargetFPS(Const::TargetFPS);
//    SetExitKey(KEY_NULL);
//
//    Snake snake(Const::CellSize * (Const::BoardWidth / 2), Const::CellSize * (Const::BoardHeight / 2));
//    Rectangle apple = GetApple(snake);
//
//    Score score;
//    EndText doneTxt;
//    bool finished = false;
//    bool reset = false;
//
//    while (!WindowShouldClose())
//    {
//        const float deltaTime = GetFrameTime();
//        if (!finished && snake.HandleInput(deltaTime))
//            finished = true;
//
//        if (!finished && CheckCollisionRecs(apple, snake.GetHead()))
//        {
//            if (!snake.Append())
//            {
//                doneTxt.SetWin();
//
//                apple.x = Const::AppleWinOffset;
//                apple.y = Const::AppleWinOffset;
//                finished = true;
//            }
//            else
//                apple = GetApple(snake);
//            ++score;
//        }
//
//        BeginDrawing();
//        ClearBackground(backgroundColor);
//        if(!finished)
//        {
//            for (const Rectangle& cell : grid)
//            {
//                DrawRectangleLinesEx(cell, Const::GridOutlineThickness, WHITE);
//            }
//        }
//        else
//        {
//            for (const Rectangle& cell : grid)
//            {
//                DrawRectangleLinesEx(cell, Const::GridOutlineThickness, CLITERAL(Color) { 255, 255, 255, 75 });
//            }
//        }
//        DrawRectangleRec(apple, RED);
//        snake.Draw();
//        score.Draw();
//        doneTxt.Draw(finished);
//        DrawFPS(0, 0);
//        EndDrawing();
//
//        if (finished && GetKeyPressed() == KEY_SPACE)
//        {
//            snake.Reset();
//            apple = GetApple(snake);
//   
//            score = 0;
//            reset = true;
//        }
//        if (reset && doneTxt.Reset(deltaTime))
//        {
//            reset = false;
//            finished = false;
//        }
//    }
//    
//    TerminateWindow();
//    return 0;
//}
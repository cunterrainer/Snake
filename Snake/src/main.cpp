#include <iostream>
#include <array>
#include <string>

#include "raylib.h"

#include "Snake.h"
#include "EndText.h"
#include "DisplayText.h"
#include "Constants.h"
#include "Score.h"
#include "Clang.h"


constexpr std::array<Rectangle, Const::GridSize> GenerateGrid()
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


inline int GetApplePos(int cells)
{
    const int value = GetRandomValue(0, cells * Const::CellSize);
    const int modCellSize = value % Const::CellSize;
    if (modCellSize == 0)
        return value;

    // e.g. v = 74, cs = 50 => mcs = 24 ==> round down
    const int remainder = Const::CellSize - modCellSize;
    if (remainder > Const::CellSize % 2)
        return value - modCellSize;

    // e.g. v = 76, cs = 50 => mcs = 26 ==> round up
    // cs - mcs = 24 => v + 24 = 100
    return value + remainder;
}


inline Rectangle GetApple(const Snake& snake)
{
    Rectangle apple;
    apple.width = Const::CellSize;
    apple.height = Const::CellSize;

    // TODO: keep track of empty cells than randomly choose one to spawn the apple,
    // since this loop can take very long if only 1 cell is left
    // However we can leave it this way until I encounter that it takes a very long time
    do
    {
        apple.x = static_cast<float>(GetApplePos(Const::BoardWidth - 1));
        apple.y = static_cast<float>(GetApplePos(Const::BoardHeight - 1));
    } while (snake.Collision(apple));
    return apple;
}


int main()
{
    static constexpr std::array<Rectangle, Const::GridSize> grid = GenerateGrid();
    InitWindow(Const::WindowWidth, Const::WindowHeight, Const::WindowTitle);
    SetTargetFPS(Const::TargetFPS);
    SetExitKey(KEY_NULL);

    Snake snake(Const::CellSize * (Const::BoardWidth / 2), Const::CellSize * (Const::BoardHeight / 2));
    Rectangle apple = GetApple(snake);

    Score score;
    EndText doneTxt;
    bool finished = false;

    while (!WindowShouldClose())
    {
        if (!finished && snake.HandleInput(GetFrameTime()))
            finished = true;

        if (!finished && CheckCollisionRecs(apple, snake.GetHead()))
        {
            if (!snake.Append())
            {
                doneTxt.SetWin();

                apple.x = Const::AppleWinOffset;
                apple.y = Const::AppleWinOffset;
                finished = true;
            }
            else
                apple = GetApple(snake);
            ++score;
        }

        BeginDrawing();
        ClearBackground(CLITERAL(Color) { 10, 10, 10, 255 });
        if(!finished)
        {
            for (const Rectangle& cell : grid)
            {
                DrawRectangleLinesEx(cell, Const::GridOutlineThickness, WHITE);
            }
        }
        else
        {
            for (const Rectangle& cell : grid)
            {
                DrawRectangleLinesEx(cell, Const::GridOutlineThickness, CLITERAL(Color) { 255, 255, 255, 75 });
            }
        }
        DrawRectangleRec(apple, RED);
        snake.Draw();
        score.Draw();
        doneTxt.Draw(finished);
        DrawFPS(0, 0);
        EndDrawing();

        if (finished && GetKeyPressed() == KEY_SPACE)
        {
            snake.Reset();
            apple = GetApple(snake);
   
            score = 0;
            doneTxt.Reset();

            finished = false;
        }
    }
    
    TerminateWindow();
    return 0;
}
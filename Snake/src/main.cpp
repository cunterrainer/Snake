#include <iostream>
#include <array>
#include <string>

#include "raylib.h"

#include "Snake.h"
#include "Constants.h"

/*
    For some reason clang preferably links against main() if available even though the build
    settings would link against WinMain().
    However in order to link against WinMain() I created this macro.
    GCC & MSVC properly link against WinMain().
    Premake will define all these macros for you if you select clang as compiler.
*/
#if defined CLANG && defined RELEASE && defined WINDOWS
    #define main() WinMain([[maybe_unused]]void* hInst, [[maybe_unused]]void* hInstPrev, [[maybe_unused]]char* cmdline, [[maybe_unused]]int cmdshow)
#endif


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


int GetApplePos(int cells);
Rectangle GetApple(const Snake& snake);
int GetCenterXPosText(const std::string& text, int fontSize, uint16_t width);
int GetCenterYPosText(int fontSize, uint16_t height);


int GetApplePos(int cells)
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


Rectangle GetApple(const Snake& snake)
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


int GetCenterXPosText(const std::string& text, int fontSize, uint16_t width)
{
    const int textLength = MeasureText(text.c_str(), fontSize);
    return static_cast<int>(static_cast<float>(width) / 2.f - static_cast<float>(textLength) / 2.f);
}


int GetCenterYPosText(int fontSize, uint16_t height)
{
    return static_cast<int>(static_cast<float>(height) / 2.f - static_cast<float>(fontSize) / 2.f);
}


int main()
{
    constexpr std::array<Rectangle, Const::GridSize> grid = GenerateGrid();
    InitWindow(Const::WindowWidth, Const::WindowHeight, Const::WindowTitle);
    SetTargetFPS(Const::TargetFPS);
    SetExitKey(KEY_NULL);

    Snake snake(Const::CellSize * (Const::BoardWidth / 2), Const::CellSize * (Const::BoardHeight / 2));
    Rectangle apple = GetApple(snake);

    uint16_t score = 0;
    std::string scoreStr = std::to_string(static_cast<int>(score));
    int scoreTextPosX = GetCenterXPosText(scoreStr, Const::ScoreFontSize, Const::WindowWidth);

    bool finished = false;
    std::string finishStr = "Failed!";
    int finishTextPosX = GetCenterXPosText(finishStr, Const::DoneFontSize, Const::WindowWidth);
    int finishTextPosY = GetCenterYPosText(Const::DoneFontSize, Const::WindowHeight);
    Color finishTextColor = RED;

    while (!WindowShouldClose())
    {
        if (!finished && snake.HandleInput(GetFrameTime()))
            finished = true;

        if (!finished && CheckCollisionRecs(apple, snake.GetHead()))
        {
            if (!snake.Append())
            {
                finishStr = "Won!";
                finishTextPosX = GetCenterXPosText(finishStr, Const::DoneFontSize, Const::WindowWidth);
                finishTextPosY = GetCenterYPosText(Const::DoneFontSize, Const::WindowHeight);
                finishTextColor = GOLD;

                apple.x = Const::AppleWinOffset;
                apple.y = Const::AppleWinOffset;
                finished = true;
            }
            else
                apple = GetApple(snake);
            ++score;
            scoreStr = std::to_string(static_cast<int>(score));
            scoreTextPosX = GetCenterXPosText(scoreStr, Const::ScoreFontSize, Const::WindowWidth);
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
        DrawText(scoreStr.c_str(), scoreTextPosX, 0, Const::ScoreFontSize, DARKGREEN);
        if(finished)
            DrawText(finishStr.c_str(), finishTextPosX, finishTextPosY, Const::DoneFontSize, finishTextColor);
        DrawFPS(0, 0);
        EndDrawing();

        if (finished && GetKeyPressed() == KEY_SPACE)
        {
            snake.Reset();
            apple = GetApple(snake);
        
            // reset score
            score = 0;
            scoreStr = std::to_string(static_cast<int>(score));
            scoreTextPosX = GetCenterXPosText(scoreStr, Const::ScoreFontSize, Const::WindowWidth);
        
            // reset finished string
            finishStr = "Failed!";
            finishTextPosX = GetCenterXPosText(finishStr, Const::DoneFontSize, Const::WindowWidth);
            finishTextPosY = GetCenterYPosText(Const::DoneFontSize, Const::WindowHeight);
            finishTextColor = RED;
        
            finished = false;
        }
    }
    
    TerminateWindow();
    return 0;
}
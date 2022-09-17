#pragma once
#include <array>
#include <string>

#include "raylib.h"

#include "Snake.h"
#include "EndText.h"
#include "DisplayText.h"
#include "Constants.h"
#include "Portal.h"
#include "Score.h"
#include "Layer.h"


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
        apple.x = static_cast<float>(Utility::GetRandomCell(Const::BoardWidth - 1));
        apple.y = static_cast<float>(Utility::GetRandomCell(Const::BoardHeight - 1));
    } while (snake.Collision(apple));
    return apple;
}


class Game final : public Layer
{
private:
    Snake snake{ Const::CellSize * (Const::BoardWidth / 2), Const::CellSize * (Const::BoardHeight / 2) };
    Rectangle apple = GetApple(snake);
    Portal m_Portal{snake};

    Score score;
    bool m_Won = false;
    bool finished = false;
private:
    inline void Finish(bool removeApple = false)
    {
        if (removeApple)
        {
            apple.x = Const::AppleWinOffset;
            apple.y = Const::AppleWinOffset;
        }

        finished = true;
        Reset();
    }


    inline void Reset()
    {
        snake.Reset();
        apple = GetApple(snake);
        m_Portal.Reset();
        score = 0;
    }
public:
    inline void SetWin(bool) override 
    {
        /*Get win value of Menu which always return false as win*/
    }


    inline void OnKeyPress(int keyPressed, float dt) override
    {
        if (!finished && snake.HandleInput(keyPressed, dt))
            Finish();
    }


    inline void OnUpdate(float) override
    {
        if (!finished && CheckCollisionRecs(apple, snake.GetHead()))
        {
            if (!snake.Append())
            {
                m_Won = true;
                Finish(true);
            }
            else
                apple = GetApple(snake);
            ++score;
        }

        if (!finished && CheckCollisionRecs(snake.GetHead(), m_Portal.GetIn()))
        {
            snake.SetHead(m_Portal.GetOut());
            m_Portal.Reset();
        }
    }


    inline void OnRender() const override
    {
        DrawRectangleRec(apple, RED);
        m_Portal.Draw();
        snake.Draw();
        score.Draw();
        DrawFPS(0, 0);
    }


    inline bool Win() const override
    {
        return m_Won;
    }


    inline LayerStage Done() override
    {
        const LayerStage returnStage = finished ? LayerStage::Done : LayerStage::NotDone;
        if (finished)
            finished = false;
        return returnStage;
    }
};
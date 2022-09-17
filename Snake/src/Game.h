#pragma once
#include <array>

#include "raylib.h"

#include "Snake.h"
#include "Constants.h"
#include "Utility.h"
#include "Portal.h"
#include "Score.h"
#include "Layer.h"


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
    LayerStage m_Stage = LayerStage::NotDone;
    bool m_Won = false;
private:
    inline void Finish(bool removeApple = false)
    {
        if (removeApple)
        {
            apple.x = Const::AppleWinOffset;
            apple.y = Const::AppleWinOffset;
        }

        m_Stage = LayerStage::Done;
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
        /*Gets win value of Menu which always returns false as win*/
    }


    inline void OnKeyPress(int keyPressed, float dt) override
    {
        if (m_Stage != LayerStage::NotDone)
            return;

        if (snake.HandleInput(keyPressed, dt)) // collision
            Finish();
    }


    inline void OnUpdate(float) override
    {
        if (m_Stage != LayerStage::NotDone)
            return;

        if (CheckCollisionRecs(apple, snake.GetHead()))
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

        if (CheckCollisionRecs(snake.GetHead(), m_Portal.GetIn()))
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
        const LayerStage returnStage = m_Stage == LayerStage::Done ? LayerStage::Done : LayerStage::NotDone;
        m_Stage = LayerStage::NotDone;
        return returnStage;
    }
};
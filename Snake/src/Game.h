#pragma once
#include <vector>

#include "raylib.h"

#include "Snake.h"
#include "Constants.h"
#include "Utility.h"
#include "Portal.h"
#include "Score.h"
#include "Layer.h"


class Game final : public Layer
{
private:
    Snake snake{ Const::CellSize * (Const::BoardWidth / 2), Const::CellSize * (Const::BoardHeight / 2) };
    Rectangle apple{0, 0, Const::CellSize, Const::CellSize};
    Portal m_Portal;
    std::vector<Rectangle> m_EmptyCells;

    Score score;
    LayerStage m_Stage = LayerStage::NotDone;
    bool m_Won = false;
private:
    inline void ResetPortal(bool setEmptyCells)
    {
        if(setEmptyCells)
            SetEmptyCells();
        m_Portal.Reset(m_EmptyCells);
    }


    inline void ResetApple()
    {
        SetEmptyCells();
        apple = Utility::GetRandomCell(m_EmptyCells);
    }


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
        ResetApple();
        ResetPortal(false);
        score = 0;
    }


    inline void InitEmptyCells()
    {
        m_EmptyCells.clear();
        for (const Rectangle& cell : Const::Grid)
            m_EmptyCells.emplace_back(cell);
    }


    inline void SetEmptyCells()
    {
        InitEmptyCells();
        for (size_t i = 0; i < m_EmptyCells.size(); ++i)
        {
            const Rectangle& cell = m_EmptyCells[i];
            if (CheckCollisionRecs(apple, cell) || m_Portal.Collision(cell) || snake.Collision(cell))
            {
                m_EmptyCells.erase(m_EmptyCells.begin() + static_cast<std::vector<Rectangle>::const_iterator::difference_type>(i));
                --i;
            }
        }
    }
public:
    Game()
    {
        m_EmptyCells.reserve(Const::Grid.size());
        ResetApple();
        ResetPortal(false);
    }


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
                ResetApple();
            ++score;
        }
        else if (m_Portal.SnakeCollision(snake))
            ResetPortal(true);

        if (m_EmptyCells.size() <= Const::PortalLimit)
            m_Portal.Hide();
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
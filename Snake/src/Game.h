#pragma once
#include <vector>

#include "raylib.h"

#include "Snake.h"
#include "Constants.h"
#include "Utility.h"
#include "Portal.h"
#include "Score.h"
#include "Layer.h"
#include "Sprite.h"
#include "Walls.h"
#include "AppleSprite.h"


class Game final : public Layer
{
private:
    Snake snake{ Const::CellSize * (Const::BoardWidth / 2), Const::CellSize * (Const::BoardHeight / 2) };
    Rectangle apple{0, 0, Const::CellSize, Const::CellSize};
    Portal m_Portal;
    Walls m_Walls;
    std::vector<Rectangle> m_EmptyCells;

    Score score;
    LayerStage m_Stage = LayerStage::NotDone;
    bool m_Won = false;

    Sprite m_AppleSprite;
private:
    inline void ResetPortal()
    {
        m_Portal.Reset(m_EmptyCells);
    }


    inline void ResetApple()
    {
        apple = Utility::GetRandomCell(m_EmptyCells);
    }


    inline void Finish()
    {
        m_Stage = LayerStage::Done;
        snake.Reset();
        SetEmptyCells();
        m_Walls.Init(m_EmptyCells);
        ResetApple();
        ResetPortal();
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
            if (CheckCollisionRecs(apple, cell) || m_Portal.Collision(cell) || m_Walls.Collision(cell) || snake.Collision(cell))
            {
                m_EmptyCells.erase(m_EmptyCells.begin() + static_cast<std::vector<Rectangle>::const_iterator::difference_type>(i));
                --i;
            }
        }
    }
public:
    Game()
    {
        m_AppleSprite.Load(".png", Const::SpriteSheet::RawAppleSpriteData, Const::SpriteSheet::RawAppleSpriteDataRelativeSize);
        m_AppleSprite.MakeFit();
        m_EmptyCells.reserve(Const::Grid.size());

        SetEmptyCells();
        m_Walls.Init(m_EmptyCells);
        ResetApple();
        ResetPortal();
    }


    inline void SetWin(bool) override 
    {
        /*Gets win value of Menu which always returns false as win*/
    }


    inline void OnKeyPress(int keyPressed, float dt) override
    {
        if (snake.HandleInput(keyPressed, dt)) // collision
            Finish();
    }


    inline void OnUpdate(float) override
    {
        if (m_Stage != LayerStage::NotDone)
            return;

        if (CheckCollisionRecs(apple, snake.GetHead()))
        {
            snake.Append();
            SetEmptyCells();
            if (m_EmptyCells.empty())
            {
                m_Won = true;
                Finish();
            }
            else
                ResetApple();
            ++score;
        }
        else if (m_Portal.SnakeCollision(snake))
        {
            SetEmptyCells();
            ResetPortal();
        }
        else if (m_Walls.Collision(snake.GetHead()))
        {
            Finish();
        }
    }


    inline void OnRender() const override
    {
        m_AppleSprite.Draw(apple);
        m_Portal.Draw();
        m_Walls.Draw();
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
#pragma once
#include <vector>

#include "raylib.h"

#include "Constants.h"
#include "Utility.h"
#include "Snake.h"


class Portal
{
private:
    Rectangle m_First { 0, 0, Const::CellSize, Const::CellSize };
    Rectangle m_Second{ 0, 0, Const::CellSize, Const::CellSize };
public:
    inline const Rectangle& GetFirst()  const { return m_First; }
    inline const Rectangle& GetSecond() const { return m_Second; }

    inline bool Collision(const Rectangle& rect) const
    {
        return CheckCollisionRecs(rect, m_First) || CheckCollisionRecs(rect, m_Second);
    }


    inline void Reset(std::vector<Rectangle>& emptyCells)
    {
        m_First  = Utility::GetRandomCell(emptyCells);
        m_Second = Utility::GetRandomCell(emptyCells);
    }


    inline bool SnakeCollision(Snake& snake) const
    {
        if (CheckCollisionRecs(snake.GetHead(), m_First))
        {
            snake.SetHead(m_Second);
            return true;
        }
        else if (CheckCollisionRecs(snake.GetHead(), m_Second))
        {
            snake.SetHead(m_First);
            return true;
        }
        return false;
    }


    inline void Draw() const
    {
        DrawRectangleRec(m_First, DARKBLUE);
        DrawRectangleRec(m_Second, DARKBLUE);
    }
};
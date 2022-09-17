#pragma once
#include <vector>

#include "raylib.h"

#include "Constants.h"
#include "Utility.h"
#include "Snake.h"


class Portal
{
private:
    Rectangle m_First{ Const::PortalWinOffset, Const::PortalWinOffset, Const::CellSize, Const::CellSize };
    Rectangle m_Second{ Const::PortalWinOffset, Const::PortalWinOffset, Const::CellSize, Const::CellSize };
public:
    inline const Rectangle& GetFirst()  const { return m_First; }
    inline const Rectangle& GetSecond() const { return m_Second; }

    inline bool Collision(const Rectangle& rect) const
    {
        return CheckCollisionRecs(rect, m_First) || CheckCollisionRecs(rect, m_Second);
    }


    inline void Hide()
    {
        m_First.x = Const::PortalWinOffset;
        m_First.y = Const::PortalWinOffset;
        m_Second.x = Const::PortalWinOffset;
        m_Second.y = Const::PortalWinOffset;
    }


    inline void Reset(std::vector<Rectangle>& emptyCells)
    {
        if (emptyCells.size() > Const::PortalLimit)
        {
            m_First = Utility::GetRandomCell(emptyCells);
            m_Second = Utility::GetRandomCell(emptyCells);
        }
        else
            Hide();
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
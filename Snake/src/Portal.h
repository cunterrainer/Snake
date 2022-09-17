#pragma once
#include <vector>

#include "raylib.h"

#include "Constants.h"
#include "Utility.h"
#include "Snake.h"


class Portal
{
private:
    Rectangle m_In { Const::PortalWinOffset, Const::PortalWinOffset, Const::CellSize, Const::CellSize };
    Rectangle m_Out{ Const::PortalWinOffset, Const::PortalWinOffset, Const::CellSize, Const::CellSize };
public:
    inline const Rectangle& GetIn()  const { return m_In;  }
    inline const Rectangle& GetOut() const { return m_Out; }

    inline bool Collision(const Rectangle& rect) const
    {
        return CheckCollisionRecs(rect, m_In) || CheckCollisionRecs(rect, m_Out);
    }


    inline void Hide()
    {
        m_In.x  = Const::PortalWinOffset;
        m_In.y  = Const::PortalWinOffset;
        m_Out.x = Const::PortalWinOffset;
        m_Out.y = Const::PortalWinOffset;
    }


    inline void Reset(std::vector<Rectangle>& emptyCells)
    {
        if (emptyCells.size() > Const::PortalLimit)
        {
            m_In = Utility::GetRandomCell(emptyCells);
            m_Out = Utility::GetRandomCell(emptyCells);
        }
        else
            Hide();
    }


    inline void Draw() const
    {
        DrawRectangleRec(m_In, SKYBLUE);
        DrawRectangleRec(m_Out, DARKBLUE);
    }
};
#pragma once
#include <vector>

#include "raylib.h"

#include "Constants.h"
#include "Utility.h"
#include "Snake.h"
#include "Sprite.h"
#include "PortalSpriteSheet.h"

class Portal
{
private:
    Rectangle m_First { 0, 0, Const::CellSize, Const::CellSize };
    Rectangle m_Second{ 0, 0, Const::CellSize, Const::CellSize };

    Sprite m_PortalSprite;
private:
    inline const Rectangle& GetRandomPortalSprite() const
    {
        switch (GetRandomValue(1, 4))
        {
        case 1:
            return Const::Sprite::PortalBlue;
        case 2:
            return Const::Sprite::PortalRed;
        case 3:
            return Const::Sprite::PortalViolet;
        default:
            return Const::Sprite::PortalGreen;
        }
    }
public:
    Portal()
    {
        m_PortalSprite.Load(".png", Const::SpriteSheet::RawPortalSpriteSheetData, Const::SpriteSheet::RawPortalSpriteSheetDataRelativeSize);
        m_PortalSprite.Crop(GetRandomPortalSprite());
    }

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

        m_PortalSprite.Crop(GetRandomPortalSprite());
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
        m_PortalSprite.Draw(m_First);
        m_PortalSprite.Draw(m_Second);
        //DrawRectangleRec(m_First, DARKBLUE);
        //DrawRectangleRec(m_Second, DARKBLUE);
    }
};
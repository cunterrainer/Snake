#pragma once
#include "raylib.h"

#include "Constants.h"
#include "Utility.h"
#include "Snake.h"

class Portal
{
private:
    const Snake& m_SnakeRef;
    Rectangle m_In = GeneratePortal();
    Rectangle m_Out = GeneratePortal();
private:
    inline Rectangle GeneratePortal() const
    {
        Rectangle portal;
        portal.width = Const::CellSize;
        portal.height = Const::CellSize;

        // TODO: keep track of empty cells than randomly choose one to spawn the portal,
        // since this loop can take very long if only 1 cell is left
        // However we can leave it this way until I encounter that it takes a very long time
        do
        {
            portal.x = static_cast<float>(Utility::GetRandomCell(Const::BoardWidth - 1));
            portal.y = static_cast<float>(Utility::GetRandomCell(Const::BoardHeight - 1));
        } while (m_SnakeRef.Collision(portal) || CheckCollisionRecs(portal, m_In));
        return portal;
    }
public:
    explicit Portal(const Snake& snake) : m_SnakeRef(snake) {}

    inline const Rectangle& GetIn() const { return m_In; }
    inline const Rectangle& GetOut() const { return m_Out; }

    inline void Reset()
    {
        m_In = GeneratePortal();
        m_Out = GeneratePortal();
    }

    inline void Draw() const
    {
        DrawRectangleRec(m_In, SKYBLUE);
        DrawRectangleRec(m_Out, DARKBLUE);
    }
};
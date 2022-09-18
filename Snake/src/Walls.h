#pragma once
#include <vector>
#include <algorithm>

#include "raylib.h"

#include "Constants.h"
#include "Utility.h"

class Walls
{
private:
    std::vector<Rectangle> m_Bricks;
public:
    inline void Init(std::vector<Rectangle>& emptyCells)
    {
        m_Bricks.clear();
        const size_t numOfBricks = GetRandomValue(Const::BricksMin, Const::BricksMax);
        if (emptyCells.size() <= numOfBricks)
            return;

        for (size_t i = 0; i < numOfBricks; ++i)
            m_Bricks.emplace_back(Utility::GetRandomCell(emptyCells));
    }


    inline bool Collision(const Rectangle& rec) const
    {
        return std::any_of(m_Bricks.cbegin(), m_Bricks.cend(), [&](const Rectangle& brick) { return CheckCollisionRecs(rec, brick); });
    }


    inline void Draw() const
    {
        for (const Rectangle& rec : m_Bricks)
        {
            DrawRectangleRec(rec, GRAY);
        }
    }
};
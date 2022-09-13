#pragma once
#include <vector>
#include <algorithm>

#include "raylib.h"

#include "Constants.h"

class Snake final
{
private:
    std::vector<Rectangle> m_Parts;
    std::vector<Rectangle> m_OldParts;
    int m_LastValidPressedKey = 0;
    int m_LastPressedKey = 0;

    const uint16_t m_XStart;
    const uint16_t m_YStart;
private:
    static Rectangle CreatePart(uint16_t x, uint16_t y)
    {
        Rectangle rect;
        rect.x = x;
        rect.y = y;
        rect.width = Const::CellSize;
        rect.height = Const::CellSize;
        return rect;
    }


    inline Rectangle MoveHead(int keyPressed)
    {
        static uint8_t recursionDepth = 0;
        ++recursionDepth;

        Rectangle head = m_Parts.front();
        switch (keyPressed)
        {
        case KEY_W:
        case KEY_UP:
            head.y -= Const::CellSize;
            break;
        case KEY_A:
        case KEY_LEFT:
            head.x -= Const::CellSize;
            break;
        case KEY_S:
        case KEY_DOWN:
            head.y += Const::CellSize;
            break;
        case KEY_D:
        case KEY_RIGHT:
            head.x += Const::CellSize;
            break;
        default:
            if (recursionDepth == 2)
                return head;
            return MoveHead(m_LastValidPressedKey);
        }
        recursionDepth = 0;
        m_LastValidPressedKey = keyPressed;
        return head;
    }


    inline void Move(int keyPressed)
    {
        const Rectangle head = MoveHead(keyPressed);

        m_OldParts = m_Parts;
        m_Parts.front() = head;
        for (size_t i = 1; i < m_OldParts.size(); ++i)
        {
            m_Parts[i] = m_OldParts[i - 1];
        }
    }
public:
    explicit Snake(uint16_t xStartPos, uint16_t yStartPos)
        : m_XStart(xStartPos), m_YStart(yStartPos)
    {
        m_Parts.reserve(Const::GridSize + 1);
        m_Parts.push_back(CreatePart(xStartPos, yStartPos));

        m_OldParts = m_Parts;
        m_OldParts.reserve(Const::GridSize + 1);
    }


    inline Rectangle GetHead() const { return m_Parts.front(); }
    inline bool Append() { m_Parts.push_back(m_OldParts.back()); return m_Parts.size() != Const::GridSize; }


    inline bool Collision(const Rectangle& extRect) const
    {
        return std::any_of(m_Parts.cbegin(), m_Parts.cend(), [&](const Rectangle& rect) { return CheckCollisionRecs(extRect, rect); });
    }


    inline bool SelfCollisionOrOutOfBounds() const
    {
        const size_t mPartsSize = m_Parts.size();
        for (size_t i = 0; i < mPartsSize; ++i)
        {
            const Rectangle& rect = m_Parts[i];
            if (rect.x >= Const::WindowWidth || rect.x < 0 || rect.y >= Const::WindowHeight || rect.y < 0)
                return true;

            for (size_t k = 0; k < mPartsSize; ++k)
            {
                if(i != k && CheckCollisionRecs(rect, m_Parts[k]))
                    return true;
            }
        }
        return false;
    }


    inline void Reset()
    {
        m_Parts.clear();
        m_Parts.push_back(CreatePart(m_XStart, m_YStart));
        m_OldParts = m_Parts;
        m_LastValidPressedKey = KEY_NULL;
        m_LastPressedKey = KEY_NULL;
    }


    inline void Draw() const
    {
        for(size_t i = 1; i < m_Parts.size(); ++i)
            DrawRectangleRec(m_Parts[i], GREEN);
        DrawRectangleRec(m_Parts.front(), DARKGREEN);
    }


    inline bool HandleInput(int keyPressed, float deltaTime)
    {
        static float elapsedTime = 0.f;
        elapsedTime += deltaTime;

        m_LastPressedKey = keyPressed == KEY_NULL ? m_LastPressedKey : keyPressed;
        
        if (elapsedTime >= Const::MoveDeltaTimeLimit)
        {
            Move(m_LastPressedKey);
            elapsedTime = 0.f;
            return SelfCollisionOrOutOfBounds();
        }
        return false;
    }
};
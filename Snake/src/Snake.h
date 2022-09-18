#pragma once
#include <vector>
#include <algorithm>

#include "raylib.h"

#include "Constants.h"
#include "Sprite.h"
#include "Utility.h"

class Snake final
{
private:
    enum class State
    {
        Up, Down, Left, Right
    };

    struct PartState
    {
        Rectangle pos;
        State state;
    };

    std::vector<PartState> m_Parts;
    std::vector<PartState> m_OldParts;
    int m_LastValidPressedKey = 0;
    int m_LastPressedKey = 0;

    const uint16_t m_XStart;
    const uint16_t m_YStart;

    Sprite m_HeadSprite;
    Sprite m_BodySprite;
private:
    static PartState CreatePart(uint16_t x, uint16_t y)
    {
        Rectangle rect;
        rect.x = x;
        rect.y = y;
        rect.width = Const::CellSize;
        rect.height = Const::CellSize;
        return { rect, State::Up };
    }


    inline PartState MoveHead(int keyPressed)
    {
        static uint8_t recursionDepth = 0;
        ++recursionDepth;

        PartState head = m_Parts.front();
        switch (keyPressed)
        {
        case KEY_W:
        case KEY_UP:
            head.pos.y -= Const::CellSize;
            head.state = State::Up;
            m_HeadSprite.Crop(Const::Sprite::HeadUp);
            break;
        case KEY_A:
        case KEY_LEFT:
            head.pos.x -= Const::CellSize;
            head.state = State::Left;
            m_HeadSprite.Crop(Const::Sprite::HeadLeft);
            break;
        case KEY_S:
        case KEY_DOWN:
            head.pos.y += Const::CellSize;
            head.state = State::Down;
            m_HeadSprite.Crop(Const::Sprite::HeadDown);
            break;
        case KEY_D:
        case KEY_RIGHT:
            head.pos.x += Const::CellSize;
            head.state = State::Right;
            m_HeadSprite.Crop(Const::Sprite::HeadRight);
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
        const PartState head = MoveHead(keyPressed);
        if (Utility::RectanglesAreSame(head.pos, m_Parts.front().pos))
            return;

        m_OldParts = m_Parts;
        m_Parts.front() = head;
        for (size_t i = 1; i < m_OldParts.size(); ++i)
        {
            m_Parts[i] = m_OldParts[i - 1];
        }
    }
public:
    inline explicit Snake(uint16_t xStartPos, uint16_t yStartPos)
        : m_XStart(xStartPos), m_YStart(yStartPos)
    {
        m_Parts.reserve(Const::GridSize + 1);
        m_Parts.push_back(CreatePart(xStartPos, yStartPos));
        m_Parts.push_back(CreatePart(xStartPos, yStartPos + Const::CellSize));

        m_OldParts = m_Parts;
        m_OldParts.reserve(Const::GridSize + 1);

        m_HeadSprite.Load("res/headSprites.png");
        m_HeadSprite.Crop(Const::Sprite::HeadUp);

        m_BodySprite.Load("res/bodySprites.png");
        m_BodySprite.Crop(Const::Sprite::BodyVertical);
    }


    inline Rectangle GetHead() const { return m_Parts.front().pos; }
    inline void SetHead(const Rectangle& rect) { m_Parts.front().pos = rect; }
    inline bool Append() { m_Parts.push_back(m_OldParts.back()); return m_Parts.size() != Const::GridSize; }


    inline bool Collision(const Rectangle& extRect) const
    {
        return std::any_of(m_Parts.cbegin(), m_Parts.cend(), [&](const PartState& pState) { return CheckCollisionRecs(extRect, pState.pos); });
    }


    inline bool SelfCollisionOrOutOfBounds() const
    {
        const size_t mPartsSize = m_Parts.size();
        for (size_t i = 0; i < mPartsSize; ++i)
        {
            const Rectangle& rect = m_Parts[i].pos;
            if (rect.x >= Const::WindowWidth || rect.x < 0 || rect.y >= Const::WindowHeight || rect.y < 0)
                return true;

            for (size_t k = 0; k < mPartsSize; ++k)
            {
                if(i != k && CheckCollisionRecs(rect, m_Parts[k].pos))
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
        m_HeadSprite.Draw(m_Parts.front().pos);
        for (size_t i = 1; i < m_Parts.size(); ++i)
        {
            if (m_Parts[i].state == State::Up || m_Parts[i].state == State::Down)
            {
                m_BodySprite.Crop(Const::Sprite::BodyVertical);
            }
            else
            {
                m_BodySprite.Crop(Const::Sprite::BodyHorizontal);
            }
            m_BodySprite.Draw(m_Parts[i].pos);
        }
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
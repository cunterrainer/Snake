#pragma once
#include "raylib.h"

namespace Const::Sprite
{
    //inline constexpr Rectangle HeadUp   { 194,  2, 60, 62 };
    //inline constexpr Rectangle HeadDown { 258, 64, 60, 62 };
    //inline constexpr Rectangle HeadRight{ 256,  2, 62, 60 };
    //inline constexpr Rectangle HeadLeft { 194, 66, 62, 60 };
    //
    //inline constexpr Rectangle TailUp   { 198, 128, 52, 59 };
    //inline constexpr Rectangle TailDown { 262, 197, 52, 59 };
    //inline constexpr Rectangle TailRight{ 261, 134, 59, 52 };
    //inline constexpr Rectangle TailLeft { 192, 198, 59, 52 };
    //
    //inline constexpr Rectangle BodyRightToUp{ 4, 60, 61, 64 };
    //inline constexpr Rectangle BodyUpToRight{ 4,  3, 61, 64 };
    //
    //inline constexpr Rectangle Apple    {   4, 194, 56, 61 };

    inline constexpr Rectangle HeadUp   {   0,    0, 128, 128 };
    inline constexpr Rectangle HeadDown {   0,  128, 128, 128 };
    inline constexpr Rectangle HeadRight{ 128,  128, 128, 128 };
    inline constexpr Rectangle HeadLeft { 128,    0, 128, 128 };

    inline constexpr Rectangle TailUp{ 0,    0, 128, 128 };
    inline constexpr Rectangle TailDown{ 0,  128, 128, 128 };
    inline constexpr Rectangle TailRight{ 128,  128, 128, 128 };
    inline constexpr Rectangle TailLeft{ 128,    0, 128, 128 };

    inline constexpr Rectangle BodyVertical  {   0, 0, 128, 128 };
    inline constexpr Rectangle BodyHorizontal{ 128, 0, 128, 128 };
    inline constexpr Rectangle BodyUpToRight { 0, 128, 128, 128 };
    inline constexpr Rectangle BodyDownRight { 128, 128, 128, 128 };
    inline constexpr Rectangle BodyRightUp   {   0, 2*128, 128, 128 };
    inline constexpr Rectangle BodyRightDown { 128, 2*128, 128, 128 };
}


class Sprite final
{
private:
    Image m_Image;
    Texture2D m_Texture;
    mutable Rectangle m_Crop;
public:
    ~Sprite()
    {
        UnloadImage(m_Image);
        UnloadTexture(m_Texture);
    }


    inline void Load(const char* path)
    {
        m_Image = LoadImage(path);
        m_Texture = LoadTextureFromImage(m_Image);
    }


    inline void Crop(const Rectangle& rec) const
    {
        m_Crop = rec;
    }


    inline void MakeFit()
    {
        m_Crop = { 0, 0, static_cast<float>(m_Image.height), static_cast<float>(m_Image.width) };
    }


    inline void Draw(const Rectangle& pos) const
    {
        DrawTexturePro(m_Texture, m_Crop, pos, { 0, 0 }, 0.f, WHITE);
    }
};
#pragma once
#include "raylib.h"

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


    inline void Load(const char* fileType, const unsigned char* fileData, int dataSize)
    {
        m_Image = LoadImageFromMemory(fileType, fileData, dataSize);
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
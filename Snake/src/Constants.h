#pragma once
#include <cstdint>
#include <cmath>

namespace Const {
    inline constexpr uint8_t  BoardWidth    = 17;
    inline constexpr uint8_t  BoardHeight   = 17;
    inline constexpr uint8_t  TargetFPS     = 60;
    inline constexpr uint8_t  ScoreFontSize = 45;
    inline constexpr uint8_t  DoneFontSize  = 150;
    inline constexpr uint8_t  CellSize      = 50;
    inline constexpr uint8_t  PortalLimit   = 15;
    inline constexpr uint16_t GridSize      = BoardWidth * BoardHeight;
    inline constexpr uint16_t WindowWidth   = BoardWidth * CellSize;
    inline constexpr uint16_t WindowHeight  = BoardHeight * CellSize;
    inline const char* const  WindowTitle   = "Snake";
    inline constexpr float_t  EndTextYOffsetDenom  = 4.f;
    inline constexpr float_t  EndTextFadingStep    = 1.f;
    inline constexpr float_t  EndTextFadingDiffMP  = 500.f; // multiplier for delta time
    inline constexpr float_t  GridOutlineThickness = 0.5f;
    inline constexpr float_t  MoveDeltaTimeLimit   = 0.15f; // 0.15


    namespace Sprite
    {
        inline constexpr Rectangle HeadUp   {   0,    0, 128, 128 };
        inline constexpr Rectangle HeadDown {   0,  128, 128, 128 };
        inline constexpr Rectangle HeadRight{ 128,  128, 128, 128 };
        inline constexpr Rectangle HeadLeft { 128,    0, 128, 128 };

        inline constexpr Rectangle TailUp   {   0,    0, 128, 128 };
        inline constexpr Rectangle TailDown {   0,  128, 128, 128 };
        inline constexpr Rectangle TailRight{ 128,  128, 128, 128 };
        inline constexpr Rectangle TailLeft { 128,    0, 128, 128 };

        inline constexpr Rectangle BodyVertical  {   0,       0, 128, 128 };
        inline constexpr Rectangle BodyHorizontal{ 128,       0, 128, 128 };
        inline constexpr Rectangle BodyUpToRight {   0,     128, 128, 128 };
        inline constexpr Rectangle BodyDownRight { 128,     128, 128, 128 };
        inline constexpr Rectangle BodyRightUp   {   0, 2 * 128, 128, 128 };
        inline constexpr Rectangle BodyRightDown { 128, 2 * 128, 128, 128 };

        inline constexpr Rectangle PortalBlue  {   0, 0, 290, 570 };
        inline constexpr Rectangle PortalRed   { 300, 0, 290, 570 };
        inline constexpr Rectangle PortalViolet{ 600, 0, 290, 570 };
        inline constexpr Rectangle PortalGreen { 900, 0, 290, 570 };
    }
}
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
    inline constexpr uint16_t GridSize      = BoardWidth * BoardHeight;
    inline constexpr uint16_t WindowWidth   = BoardWidth * CellSize;
    inline constexpr uint16_t WindowHeight  = BoardHeight * CellSize;
    inline constexpr float_t  EndTextFadingStep    = 1.f;
    inline constexpr float_t  EndTextFadingDiffMP  = 500.f; // multiplier for delta time
    inline constexpr float_t  GridOutlineThickness = 0.5f;
    inline constexpr float_t  MoveDeltaTimeLimit   = 0.15f; // 0.15
    inline constexpr float_t  AppleWinOffset       = -10000000.f; // arbitrary value
    inline const char* const  WindowTitle   = "Snake";
}
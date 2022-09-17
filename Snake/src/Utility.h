#pragma once
#include "raylib.h"

#include "Constants.h"

namespace Utility
{
    inline int GetRandomCell(int cells)
    {
        const int value = GetRandomValue(0, cells * Const::CellSize);
        const int modCellSize = value % Const::CellSize;
        if (modCellSize == 0)
            return value;

        // e.g. v = 74, cs = 50 => mcs = 24 ==> round down
        const int remainder = Const::CellSize - modCellSize;
        if (remainder > Const::CellSize % 2)
            return value - modCellSize;

        // e.g. v = 76, cs = 50 => mcs = 26 ==> round up
        // cs - mcs = 24 => v + 24 = 100
        return value + remainder;
    }


    constexpr std::array<Rectangle, Const::GridSize> GenerateGrid()
    {
        std::array<Rectangle, Const::GridSize> grid{};
        uint16_t currentX = 0;
        uint16_t currentY = 0;

        for (size_t i = 0; i < grid.size(); ++i)
        {
            grid[i].x = currentX;
            grid[i].y = currentY;
            grid[i].width = Const::CellSize;
            grid[i].height = Const::CellSize;

            const bool endOfLine = (i + 1) % Const::BoardWidth == 0;
            currentX = endOfLine ? 0 : static_cast<uint16_t>(currentX + Const::CellSize);
            currentY = endOfLine ? static_cast<uint16_t>(currentY + Const::CellSize) : currentY;
        }
        return grid;
    }
}

namespace Const
{
    inline constexpr std::array<Rectangle, Const::GridSize> Grid = Utility::GenerateGrid();
}
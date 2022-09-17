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
}
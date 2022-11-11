#pragma once

#include "image.h"

namespace tiny 
{
    struct Line 
    {
        int x0, y0, x1, y1;
    };

    Line
    line_new(int x0, int y0, int x1, int y1);
};
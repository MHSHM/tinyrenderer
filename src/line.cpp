#include "line.h"

#include <cmath>

namespace tiny 
{
    Line
    line_new(int x0, int y0, int x1, int y1)
    {
        Line line;
        line.x0 = x0;
        line.y0 = y0;
        line.x1 = x1;
        line.y1 = y1;
        return line;
    }
};
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

    void
    line_draw(const Line& line, TGAImage* image, TGAColor color)
    {
        int dx = std::abs(line.x1 - line.x0);
        int dy = std::abs(line.y1 - line.y0);
        float step = 1.0f / std::pow(std::max(dx, dy), 2) + 0.0001f;
        for(float t = 0.0f; t <= 1.0f; t += step)
        {
            int x = (1.0f - t) * line.x0 + t * line.x1;
            int y = (1.0f - t) * line.y0 + t * line.y1;
            image->set(x, y, color);
        }
    }
};
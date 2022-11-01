#pragma once

namespace tiny 
{
    struct Triangle;

    struct AABB 
    {
        int min_x, max_x;
        int min_y, max_y;
    };

    AABB
    aabb_new(const Triangle& triangle);
};
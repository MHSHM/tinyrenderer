#pragma once

#include <vector>
#include <stdio.h>
#include <cstring>

namespace tiny
{
    struct Zbuffer
    {
        std::vector<float> depths;

        int width;
        int height;
    };

    inline static Zbuffer*
    zbuffer_new(int width, int height, float clear_value)
    {
        Zbuffer* zbuffer = new Zbuffer;
        zbuffer->depths.resize(width * height, clear_value);
        zbuffer->width = width;
        zbuffer->height = height;

        return zbuffer;
    }

    inline static void
    zbuffer_clear(Zbuffer* zbuffer, float clear_value)
    {
        memset((void*)&zbuffer->depths[0], (int)clear_value, sizeof(float) * zbuffer->width * zbuffer->height);
    }
};
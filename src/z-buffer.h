#pragma once

#include "resource.h"

#include <vector>
#include <stdio.h>
#include <cstring>

namespace tiny
{
    struct Zbuffer: Resource
    {
        std::vector<float> depths;

        int width;
        int height;
    };

    Zbuffer*
    zbuffer_new(int width, int height, float clear_value);

    void
    zbuffer_clear(Zbuffer* zbuffer, float clear_value);
};
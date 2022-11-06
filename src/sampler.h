#pragma once

#include "tgaimage.h"

// accepts u and v in range 0 - 1
TGAColor sample(TGAImage* texture, float u, float v)
{
    // uv to pixel space
    // now we uses nearest neighbour filtering
    // TODO: support bilinear filtering
    uint32_t x = texture->width() * u;
    uint32_t y = texture->height() * v;
    return texture->get(x, y);
}
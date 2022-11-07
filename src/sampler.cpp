#include "sampler.h"

namespace tiny 
{
    TGAColor sample(TGAImage* texture, float u, float v)
    {
        if(u < 0.0f) u = 0.0f;
        if(u > 1.0f) u = 1.0f;
        if(v < 0.0f) v = 0.0f;
        if(v > 1.0f) v = 1.0f;

        // uv to pixel space
        // now we uses nearest neighbour filtering
        // TODO: support bilinear filtering
        uint32_t x = texture->width() * u;
        uint32_t y = texture->height() * v;
        return texture->get(x, y);
    }
};
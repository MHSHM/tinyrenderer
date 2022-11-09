#include "z-buffer.h"

namespace tiny 
{
    Zbuffer*
    zbuffer_new(int width, int height, float clear_value)
    {
        static Zbuffer zbuffer;
        zbuffer.depths.resize(width * height, clear_value);
        zbuffer.width = width;
        zbuffer.height = height;

        return &zbuffer;
    }

    void
    zbuffer_clear(Zbuffer* zbuffer, float clear_value)
    {
        memset((void*)&zbuffer->depths[0], (int)clear_value, sizeof(float) * zbuffer->width * zbuffer->height);
    }
};
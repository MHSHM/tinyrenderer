#pragma once

#include "tgaimage.h"
#include "Math.h"

namespace tiny 
{
    struct Triangle
    {
        mathy::Vector3<float> v0;
        mathy::Vector3<float> v1;
        mathy::Vector3<float> v2;
    };

    Triangle
    triangle_new(const mathy::Vector3<float>& v0, const mathy::Vector3<float>& v1, const mathy::Vector3<float>& v2);

    void
    triangle_draw_wireframe(const Triangle& triangle, TGAImage& image, TGAColor color);

    void
    triangle_draw_fill(const Triangle& triangle, TGAImage& image, TGAColor color);
};
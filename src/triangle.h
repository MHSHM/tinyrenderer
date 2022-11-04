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

        mathy::Vector3<float> face_normal;
    };

    Triangle
    triangle_new(const mathy::Vector3<float>& v0, const mathy::Vector3<float>& v1, const mathy::Vector3<float>& v2);

    // draw triangle as a wireframe
    void
    triangle_draw_wireframe(const Triangle& triangle, TGAImage& image, const TGAColor& color);

    // draw triangle and fill it with a constant color
    void
    triangle_draw_fill(const Triangle& triangle, TGAImage& image, const TGAColor& color);

    // draw flat shaded triangle
    // shading equations are evaluated once per triangle
    void
    triangle_draw_flat_shading(const Triangle& triangle, TGAImage& image, const mathy::Vector3<float>& light_direction, const TGAColor& color);
};
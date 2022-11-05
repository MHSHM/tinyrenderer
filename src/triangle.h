#pragma once

#include "tgaimage.h"
#include "Math.h"
#include "z-buffer.h"

namespace tiny 
{
    struct Triangle
    {
        mathy::Vector3<float> v0;
        mathy::Vector3<float> v1;
        mathy::Vector3<float> v2;

        mathy::Vector3<float> n0;
        mathy::Vector3<float> n1;
        mathy::Vector3<float> n2;

        mathy::Vector3<float> face_normal;
    };

    Triangle
    triangle_new(const mathy::Vector3<float>& v0, const mathy::Vector3<float>& v1, const mathy::Vector3<float>& v2,
    const mathy::Vector3<float>& n0, const mathy::Vector3<float>& n1, const mathy::Vector3<float>& n2);

    // draw triangle as a wireframe
    void
    triangle_draw_wireframe(const Triangle& triangle, TGAImage* image, const TGAColor& color);

    // draw triangle and fill it with a constant color
    void
    triangle_draw_fill(const Triangle& triangle, TGAImage* image, const TGAColor& color);

    // draw flat shaded triangle
    // shading equations are evaluated once per triangle
    void
    triangle_draw_flat_shading(const Triangle& triangle, TGAImage* image, tiny::Zbuffer* zbuffer, const mathy::Vector3<float>& light_direction, const TGAColor& color);

    // draw shaded triangle
    // shading equations are evaluated once per pixel
    void
    triangle_draw_shading(const Triangle& triangle, TGAImage* image, tiny::Zbuffer* zbuffer, const mathy::Vector3<float>& light_direction, const TGAColor& color);
};
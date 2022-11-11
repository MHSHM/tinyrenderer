#pragma once

#include "image.h"
#include "Math.h"
#include "z-buffer.h"

namespace tiny 
{
    struct Triangle_Data
    {
        mathy::Vector3<float> v0;
        mathy::Vector3<float> v1;
        mathy::Vector3<float> v2;

        mathy::Vector3<float> n0;
        mathy::Vector3<float> n1;
        mathy::Vector3<float> n2;

        mathy::Vector2<float> uv0;
        mathy::Vector2<float> uv1;
        mathy::Vector2<float> uv2;

        mathy::Vector3<float> face_normal;  
    };

    struct Triangle
    {
        Triangle_Data data;
    };

    Triangle
    triangle_new(const Triangle_Data& data);

    // draw triangle as a wireframe
    void
    triangle_draw_wireframe(const Triangle& triangle, Image* image, const TGAColor& color);

    // draw triangle and fill it with a constant color
    void
    triangle_draw_fill(const Triangle& triangle, Image* image, const TGAColor& color);

    // draw flat shaded triangle
    // shading equations are evaluated once per triangle
    void
    triangle_draw_flat_shading(const Triangle& triangle, Image* image, tiny::Zbuffer* zbuffer, const mathy::Vector3<float>& light_direction, const TGAColor& color);

    // draw shaded triangle
    // shading equations are evaluated once per pixel
    void
    triangle_draw_shading(const Triangle& triangle, Image* image, tiny::Zbuffer* zbuffer, const mathy::Vector3<float>& light_direction, const TGAColor& color);

    // draw a triangle and apply a diffuse texture to it
    void
    triangle_draw_diffuse(const Triangle& triangle, Image* image, tiny::Zbuffer* zbuffer, const mathy::Vector3<float>& light_direction, Image* diffuse);
};
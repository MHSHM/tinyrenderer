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
};
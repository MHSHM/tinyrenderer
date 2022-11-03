#pragma once

#include "triangle.h"

namespace mathy
{
    struct Barycentric
    {
        float u, v, w;
        bool is_inside;
    };

    template<typename T>
    struct Vector2 
    {
        T x, y;

        static Vector2<T>
        vec2_new(T x , T y)
        {
            Vector2<T> v;
            v.x = x;
            v.y = y;
            return v;
        }

        inline static Vector2<T>
        cross(Vector2<T> v1, Vector2<T> v2)
        {

        }
    };
    
    
    template<typename T>
    struct Vector3
    {
        T x, y, z;

        static Vector3<T>
        vec3_new(T x , T y, T z)
        {
            Vector3<T> v;
            v.x = x;
            v.y = y;
            v.z = z;
            return v;
        }
    };

    inline Barycentric
    is_inside_triangle(const Vector2<float>& pixel, const Vector3<float>& v0, const Vector3<float>& v1, const Vector3<float>& v2)
    {
        Barycentric coord;
        coord.u = ( (v1.y - v2.y) * (pixel.x - v2.x) + (v2.x - v1.x) * (pixel.y - v2.y) ) / ( (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y) );
        coord.v = ( (v2.y - v0.y) * (pixel.x - v2.x) + (v0.x - v2.x) * (pixel.y - v2.y) ) / ( (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y) );
        coord.w = 1.0f - coord.u - coord.v;
        coord.is_inside = (coord.u >= 0.0f && coord.u <= 1.0f && coord.v >= 0.0f && coord.v <= 1.0f && coord.w >= 0.0f && coord.w <= 1.0f);
        return coord;
    }
};
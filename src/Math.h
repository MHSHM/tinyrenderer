#pragma once

namespace mathy
{
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
};
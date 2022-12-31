#pragma once

#include "triangle.h"

#include <math.h>
#include <iostream>

namespace mathy
{
    const float PI = 3.14159265359f;

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

        inline Vector2<T>
        operator-(const Vector2<T>& other) const
        {
            Vector2<T> res = vec2_new(this->x - other.x, this->y - other.y);
            return res;
        }

        inline static Vector2<T>
        cross(Vector2<T> v1, Vector2<T> v2)
        {

        }

        inline Vector2<T>
        operator*(float scalar) const
        {
            Vector2<T> res;
            res.x = this->x * scalar;
            res.y = this->y * scalar;
            return res;
        }

        inline Vector2<T>
        operator+(const Vector2<T>& other) const
        {
            Vector2<T> res;
            res.x = this->x + other.x;
            res.y = this->y + other.y;
            return res;
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

        inline static Vector3<T>
        cross(const Vector3<T>& a, const Vector3<T>& b)
        {
            T x = a.y * b.z - a.z * b.y;
            T y = a.z * b.x - a.x * b.z;
            T z = a.x * b.y - a.y * b.x;

            Vector3<T> res;
            res.x = x;
            res.y = y;
            res.z = z;

            return res;
        }

        inline static T
        dot(const Vector3<T>& v0, const Vector3<T>& v1)
        {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }

        inline static float
        magnitude(const Vector3<T>& v)
        {
            float radicand = v.x * v.x + v.y * v.y + v.z * v.z;
            return sqrt(radicand);
        }

        inline static Vector3<T>
        normalize(const Vector3<T>& v)
        {
            float v_mag = magnitude(v);

            Vector3<T> res;
            res.x = v.x / (v_mag + 0.00001f);
            res.y = v.y / (v_mag + 0.00001f);
            res.z = v.z / (v_mag + 0.00001f);

            return res;
        }

        inline Vector3<T>
        operator-(const Vector3<T>& other) const
        {
            Vector3<T> res = vec3_new(this->x - other.x, this->y - other.y, this->z - other.z);
            return res;
        }

        inline Vector3<T>
        operator-() const
        {
            Vector3<T> res;
            res.x = -this->x;
            res.y = -this->y;
            res.z = -this->z;

            return res;
        }

        inline Vector3<T>
        operator*(float scalar) const
        {
            Vector3<T> res;
            res.x = this->x * scalar;
            res.y = this->y * scalar;
            res.z = this->z * scalar;
            return res;
        }

        inline Vector3<T>
        operator+(float scalar) const
        {
            Vector3<T> res;
            res.x = this->x + scalar;
            res.y = this->y + scalar;
            res.z = this->z + scalar;
            return res;
        }

        inline Vector3<T>
        operator/(float divisior) const
        {
            Vector3<T> res;
            res.x = this->x / divisior;
            res.y = this->y / divisior;
            res.z = this->z / divisior;
            return res;
        }

        inline Vector3<T>
        operator+(const Vector3<T>& other) const
        {
            Vector3<T> res;
            res.x = this->x + other.x;
            res.y = this->y + other.y;
            res.z = this->z + other.z;
            return res;
        }

        inline Vector3<T>
        operator*(const Vector3<T>& other) const
        {
            Vector3<T> res;
            res.x = this->x * other.x;
            res.y = this->y * other.y;
            res.z = this->z * other.z;
            return res;
        }

        inline Vector3<T>
        operator/(T divisior)
        {
            Vector3<T> res;
            res.x = this->x / divisior;
            res.y = this->y / divisior;
            res.z = this->z / divisior;

            return res;
        }
    };

    inline static Barycentric
    is_inside_triangle(const Vector2<float>& pixel, const Vector3<float>& v0, const Vector3<float>& v1, const Vector3<float>& v2)
    {
        Barycentric coord;
        float denominator = ( (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y) );
        coord.u = ( (v1.y - v2.y) * (pixel.x - v2.x) + (v2.x - v1.x) * (pixel.y - v2.y) ) / denominator;
        coord.v = ( (v2.y - v0.y) * (pixel.x - v2.x) + (v0.x - v2.x) * (pixel.y - v2.y) ) / denominator;
        coord.w = 1.0f - coord.u - coord.v;
        coord.is_inside = (coord.u >= 0.0f && coord.u <= 1.0f && coord.v >= 0.0f && coord.v <= 1.0f && coord.w >= 0.0f && coord.w <= 1.0f);
        return coord;
    }

    inline static float
    to_radians(float angle_in_degrees)
    {
        return angle_in_degrees * (PI / 180.0f);
    }

    typedef Vector3<float> vec3;
    typedef Vector3<int> ivec3;
    typedef Vector2<float> vec2;
    typedef Vector2<int> ivec2;
};
#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
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

    inline static glm::vec3
    cross(const glm::vec3& a, const glm::vec3& b)
    {
        float x = a.y * b.z - a.z * b.y;
        float y = a.z * b.x - a.x * b.z;
        float z = a.x * b.y - a.y * b.x;

        glm::vec3 res;
        res.x = x;
        res.y = y;
        res.z = z;

        return res;
    }

    inline static float
    dot(const glm::vec3& v0, const glm::vec3& v1)
    {
        return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
    }

    inline static float
    magnitude(const glm::vec3& v)
    {
        float radicand = v.x * v.x + v.y * v.y + v.z * v.z;
        return sqrt(radicand);
    }

    inline static glm::vec3
    normalize(const glm::vec3& v)
    {
        float v_mag = magnitude(v);

        glm::vec3 res;
        res.x = v.x / (v_mag + 0.00001f);
        res.y = v.y / (v_mag + 0.00001f);
        res.z = v.z / (v_mag + 0.00001f);

        return res;
    }

    inline static float
    to_radians(float angle_in_degrees)
    {
        return angle_in_degrees * (PI / 180.0f);
    }

    inline static void
    scale_mat(glm::mat4& mat, const glm::vec3& scale) 
    {
        mat[0][0] = scale.x;
        mat[1][1] = scale.y;
        mat[2][2] = scale.z;
        mat[3][3] = 1.0f;
    }

    inline static void
    translation_mat(glm::mat4& mat, const glm::vec3& translation)
    {
        mat[3][0] = translation.x;
        mat[3][1] = translation.y;
        mat[3][2] = translation.z;
        mat[3][3] = 1.0f;
    }

    inline static void
    rotate_x_mat(glm::mat4& mat, float angle) 
    {
        mat[1][1] = cosf(to_radians(angle));
        mat[1][2] = sinf(to_radians(angle));
        mat[2][1] = -mat[1][2];
        mat[2][2] =  mat[1][1];
    }
 
    inline static void
    rotate_y_mat(glm::mat4& mat, float angle) 
    {
        mat[0][0] = cosf(to_radians(angle));
        mat[0][2] = sinf(to_radians(angle));
        mat[2][0] = -mat[0][2];
        mat[2][2] =  mat[0][0];
    }
 
    inline static void
    rotate_z_mat(glm::mat4& mat, float angle)
    {
        mat[0][0] = cosf(to_radians(angle));
        mat[0][1] = sinf(to_radians(angle));
        mat[1][0] = -mat[0][2];
        mat[1][1] =  mat[0][0];
    }

    // accepts only camera translation for the time being
    inline static void
    view_mat(glm::mat4& mat, const glm::vec3& translation) 
    {
        mat[3][0] = -translation.x;
        mat[3][1] = -translation.y;
        mat[3][2] = -translation.z;
    }

    inline static void
    perspective_projection_mat(glm::mat4& mat, float n, float f, float fov)
    {
        float scalar = 1.0f / tanf(to_radians(fov / 2.0f));
        mat[0][0] = scalar;
        mat[1][1] = scalar;
        mat[2][2] = -1.0f;
        mat[2][3] = -1.0f;
        mat[3][2] = 0.0f;
        mat[3][3] = 0.0f;
    }

    inline static Barycentric
    is_inside_triangle(const glm::vec2& pixel, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
    {
        Barycentric coord;
        float denominator = ( (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y) );
        coord.u = ( (v1.y - v2.y) * (pixel.x - v2.x) + (v2.x - v1.x) * (pixel.y - v2.y) ) / denominator;
        coord.v = ( (v2.y - v0.y) * (pixel.x - v2.x) + (v0.x - v2.x) * (pixel.y - v2.y) ) / denominator;
        coord.w = 1.0f - coord.u - coord.v;
        coord.is_inside = (coord.u >= 0.0f && coord.u <= 1.0f && coord.v >= 0.0f && coord.v <= 1.0f && coord.w >= 0.0f && coord.w <= 1.0f);
        return coord;
    }
};
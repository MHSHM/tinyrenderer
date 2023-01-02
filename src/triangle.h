#pragma once

#include "image.h"
#include "Math.h"
#include "z-buffer.h"

namespace tiny
{
    struct Triangle_Data
    {
        glm::vec3 v0;
        glm::vec3 v1;
        glm::vec3 v2;

        glm::vec3 n0;
        glm::vec3 n1;
        glm::vec3 n2;

        glm::vec2 uv0;
        glm::vec2 uv1;
        glm::vec2 uv2;

        glm::vec3 face_normal;
    };

    struct Triangle
    {
        Triangle_Data data;
    };

    Triangle
    triangle_new(const Triangle_Data& data);
};
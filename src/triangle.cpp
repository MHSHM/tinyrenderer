#include "triangle.h"
#include "line.h"
#include "AABB.h"
#include "sampler.h"

#include <glm/geometric.hpp>

namespace tiny
{
    Triangle
    triangle_new(const Triangle_Data& data)
    {
        Triangle triangle;
        triangle.data.v0 = data.v0;
        triangle.data.v1 = data.v1;
        triangle.data.v2 = data.v2;
        triangle.data.n0 = data.n0;
        triangle.data.n1 = data.n1;
        triangle.data.n2 = data.n2;
        triangle.data.uv0 = data.uv0;
        triangle.data.uv1 = data.uv1;
        triangle.data.uv2 = data.uv2;

        glm::vec3 e0 = triangle.data.v1 - triangle.data.v0;
        glm::vec3 e1 = triangle.data.v2 - triangle.data.v0;
        triangle.data.face_normal = mathy::cross(e0, e1);
        triangle.data.face_normal = mathy::normalize(triangle.data.face_normal);

        return triangle;
    }
};
#include "triangle.h"
#include "line.h"
#include "AABB.h"
#include "sampler.h"

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

        mathy::Vector3<float> e0 = triangle.data.v1 - triangle.data.v0;
        mathy::Vector3<float> e1 = triangle.data.v2 - triangle.data.v0;
        triangle.data.face_normal = mathy::Vector3<float>::cross(e0, e1);
        triangle.data.face_normal = mathy::Vector3<float>::normalize(triangle.data.face_normal);
        
        return triangle;
    }
};
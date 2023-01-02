#pragma once

#include "resource.h"
#include "triangle.h"

#include <vector>

namespace tiny
{
    struct Mesh : Resource
    {
        std::vector<tiny::Triangle> triangles;
    };

    Mesh*
    mesh_new();
};
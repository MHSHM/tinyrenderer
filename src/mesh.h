#pragma once

#include "resource.h"
#include "triangle.h"

#include <vector>

struct Mesh : Resource
{
    std::vector<tiny::Triangle> triangles;
};

Mesh*
mesh_new();

void
mesh_scale(Mesh* mesh, float scale_x, float scale_y, float scale_z);

void
mesh_translate(Mesh* mesh, float translate_x, float translate_y, float translate_z);
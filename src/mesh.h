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

    void
    mesh_scale(Mesh* mesh, float scale_x, float scale_y, float scale_z);

    void
    mesh_translate(Mesh* mesh, float translate_x, float translate_y, float translate_z);

    // rotate the given mesh around the x axis by the given angle
    // the angle is in radians
    void
    mesh_rotate_x(Mesh* mesh, float angle);

    // rotate the given mesh around the y axis by the given angle
    // the angle is in radians
    void
    mesh_rotate_y(Mesh* mesh, float angle);

    // rotate the given radians around the z axis by the given angle
    // the angle is in degrees
    void
    mesh_rotate_z(Mesh* mesh, float angle);

    // performs a perspective projection
    // fov is expected in degrees
    void
    mesh_z_division(Mesh* mesh, float fov);
};
#pragma once

#include "resource.h"
#include "triangle.h"

#include <string>
#include <vector>

namespace tiny
{
    struct Mesh : Resource
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;

        std::vector<tiny::Triangle> triangles;
    };

    Mesh*
    mesh_new(Resource_Manager* resource_manager, const std::string& tag);
};
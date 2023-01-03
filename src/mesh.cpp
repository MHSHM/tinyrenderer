#include "mesh.h"
#include "objloader.h"

using namespace mathy;

namespace tiny
{
    inline static void
    _load_mesh(Resource* resource, Resource_Manager* resource_manager, const std::string& path)
    {
        objl::Loader loader;
        loader.LoadFile(path);

        Mesh* mesh = (Mesh*)resource;
        mesh->triangles.resize(loader.LoadedIndices.size() / 3);
        for (int i = 0, triangle_index = 0; i < loader.LoadedIndices.size(); i += 3, triangle_index++)
        {
            tiny::Triangle_Data data;
            const std::vector<objl::Vertex>& vertices = loader.LoadedVertices;

            data.v0 = glm::vec3(vertices[loader.LoadedIndices[i]].Position.X, vertices[loader.LoadedIndices[i]].Position.Y, vertices[loader.LoadedIndices[i]].Position.Z);
            data.v1 = glm::vec3(vertices[loader.LoadedIndices[i + 1]].Position.X, vertices[loader.LoadedIndices[i + 1]].Position.Y, vertices[loader.LoadedIndices[i + 1]].Position.Z);
            data.v2 = glm::vec3(vertices[loader.LoadedIndices[i + 2]].Position.X, vertices[loader.LoadedIndices[i + 2]].Position.Y, vertices[loader.LoadedIndices[i + 2]].Position.Z);

            data.n0 = glm::vec3(vertices[loader.LoadedIndices[i]].Normal.X, vertices[loader.LoadedIndices[i]].Normal.Y, vertices[loader.LoadedIndices[i]].Normal.Z);
            data.n1 = glm::vec3(vertices[loader.LoadedIndices[i + 1]].Normal.X, vertices[loader.LoadedIndices[i + 1]].Normal.Y, vertices[loader.LoadedIndices[i + 1]].Normal.Z);
            data.n2 = glm::vec3(vertices[loader.LoadedIndices[i + 2]].Normal.X, vertices[loader.LoadedIndices[i + 2]].Normal.Y, vertices[loader.LoadedIndices[i + 2]].Normal.Z);

            data.uv0 = glm::vec2(vertices[loader.LoadedIndices[i]].TextureCoordinate.X, vertices[loader.LoadedIndices[i]].TextureCoordinate.Y);
            data.uv1 = glm::vec2(vertices[loader.LoadedIndices[i + 1]].TextureCoordinate.X, vertices[loader.LoadedIndices[i + 1]].TextureCoordinate.Y);
            data.uv2 = glm::vec2(vertices[loader.LoadedIndices[i + 2]].TextureCoordinate.X, vertices[loader.LoadedIndices[i + 2]].TextureCoordinate.Y);

            mesh->triangles[triangle_index] = tiny::triangle_new(data);
        }
    }

    inline static void
    _free_mesh(Resource* resource, Resource_Manager* resource_manager, const std::string& path)
    {
        Mesh* mesh = (Mesh*)resource;
        delete mesh;
        resource_manager_remove_resource(resource_manager, resource, path);
    }

    Mesh*
    mesh_new(Resource_Manager* resource_manager, const std::string& tag)
    {
        Mesh* mesh = new Mesh;
        mesh->load = &_load_mesh;
        mesh->free   = &_free_mesh;
        resource_manager_add_resource(resource_manager, mesh, tag);
        return mesh;
    }
};
#include "mesh.h"
#include "objloader.h"

namespace tiny
{
    inline static void
    _load_mesh(Resource* resource, Resource_Manager* resource_manager, const std::string& path)
    {
        objl::Loader loader;
        loader.LoadFile(path);

        Mesh* mesh = (Mesh*)resource;
        mesh->triangles.resize(loader.LoadedIndices.size() / 3);
        for(int i = 0, triangle_index = 0; i < loader.LoadedIndices.size(); i+=3, triangle_index++)
        {
            tiny::Triangle_Data data;
            const std::vector<objl::Vertex>& vertices = loader.LoadedVertices;

            data.v0 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i]].Position.X, vertices[loader.LoadedIndices[i]].Position.Y, vertices[loader.LoadedIndices[i]].Position.Z);
            data.v1 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i + 1]].Position.X, vertices[loader.LoadedIndices[i + 1]].Position.Y, vertices[loader.LoadedIndices[i + 1]].Position.Z);
            data.v2 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i + 2]].Position.X, vertices[loader.LoadedIndices[i + 2]].Position.Y, vertices[loader.LoadedIndices[i + 2]].Position.Z);

            data.n0 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i]].Normal.X, vertices[loader.LoadedIndices[i]].Normal.Y, vertices[loader.LoadedIndices[i]].Normal.Z);
            data.n1 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i + 1]].Normal.X, vertices[loader.LoadedIndices[i + 1]].Normal.Y, vertices[loader.LoadedIndices[i + 1]].Normal.Z);
            data.n2 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i + 2]].Normal.X, vertices[loader.LoadedIndices[i + 2]].Normal.Y, vertices[loader.LoadedIndices[i + 2]].Normal.Z);

            data.uv0 = mathy::Vector2<float>::vec2_new(vertices[loader.LoadedIndices[i]].TextureCoordinate.X, vertices[loader.LoadedIndices[i]].TextureCoordinate.Y);
            data.uv1 = mathy::Vector2<float>::vec2_new(vertices[loader.LoadedIndices[i + 1]].TextureCoordinate.X, vertices[loader.LoadedIndices[i + 1]].TextureCoordinate.Y);
            data.uv2 = mathy::Vector2<float>::vec2_new(vertices[loader.LoadedIndices[i + 2]].TextureCoordinate.X, vertices[loader.LoadedIndices[i + 2]].TextureCoordinate.Y);

            mesh->triangles[triangle_index] = tiny::triangle_new(data);
        }

        resource_manager_add_resource(resource_manager, resource, path);
    }

    inline static void
    _free_mesh(Resource* resource, Resource_Manager* resource_manager, const std::string& path)
    {
        Mesh* mesh = (Mesh*)resource;
        delete mesh;
        resource_manager_remove_resource(resource_manager, resource, path);
    }

    Mesh*
    mesh_new()
    {
        Mesh* mesh = new Mesh;
        mesh->load = &_load_mesh;
        mesh->free   = &_free_mesh;
        return mesh;
    }

    void
    mesh_scale(Mesh* mesh, float scale_x, float scale_y, float scale_z)
    {
        mathy::Vector3<float> scalar = mathy::Vector3<float>::vec3_new(scale_x, scale_y, scale_z);
        for(auto& triangle: mesh->triangles)
        {
            triangle.data.v0 = triangle.data.v0 * scalar;
            triangle.data.v1 = triangle.data.v1 * scalar;
            triangle.data.v2 = triangle.data.v2 * scalar;
        }
    }

    void
    mesh_translate(Mesh* mesh, float translate_x, float translate_y, float translate_z)
    {
        mathy::Vector3<float> translation = mathy::Vector3<float>::vec3_new(translate_x, translate_y, translate_z);
        for(auto& triangle: mesh->triangles)
        {
            triangle.data.v0 = triangle.data.v0 + translation;
            triangle.data.v1 = triangle.data.v1 + translation;
            triangle.data.v2 = triangle.data.v2 + translation;
        }
    }
};
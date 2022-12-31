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
        for(int i = 0, triangle_index = 0; i < loader.LoadedIndices.size(); i+=3, triangle_index++)
        {
            tiny::Triangle_Data data;
            const std::vector<objl::Vertex>& vertices = loader.LoadedVertices;

            data.v0 = vec3::vec3_new(vertices[loader.LoadedIndices[i]].Position.X, vertices[loader.LoadedIndices[i]].Position.Y, vertices[loader.LoadedIndices[i]].Position.Z);
            data.v1 = vec3::vec3_new(vertices[loader.LoadedIndices[i + 1]].Position.X, vertices[loader.LoadedIndices[i + 1]].Position.Y, vertices[loader.LoadedIndices[i + 1]].Position.Z);
            data.v2 = vec3::vec3_new(vertices[loader.LoadedIndices[i + 2]].Position.X, vertices[loader.LoadedIndices[i + 2]].Position.Y, vertices[loader.LoadedIndices[i + 2]].Position.Z);

            data.n0 = vec3::vec3_new(vertices[loader.LoadedIndices[i]].Normal.X, vertices[loader.LoadedIndices[i]].Normal.Y, vertices[loader.LoadedIndices[i]].Normal.Z);
            data.n1 = vec3::vec3_new(vertices[loader.LoadedIndices[i + 1]].Normal.X, vertices[loader.LoadedIndices[i + 1]].Normal.Y, vertices[loader.LoadedIndices[i + 1]].Normal.Z);
            data.n2 = vec3::vec3_new(vertices[loader.LoadedIndices[i + 2]].Normal.X, vertices[loader.LoadedIndices[i + 2]].Normal.Y, vertices[loader.LoadedIndices[i + 2]].Normal.Z);

            data.uv0 = vec2::vec2_new(vertices[loader.LoadedIndices[i]].TextureCoordinate.X, vertices[loader.LoadedIndices[i]].TextureCoordinate.Y);
            data.uv1 = vec2::vec2_new(vertices[loader.LoadedIndices[i + 1]].TextureCoordinate.X, vertices[loader.LoadedIndices[i + 1]].TextureCoordinate.Y);
            data.uv2 = vec2::vec2_new(vertices[loader.LoadedIndices[i + 2]].TextureCoordinate.X, vertices[loader.LoadedIndices[i + 2]].TextureCoordinate.Y);

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
        vec3 scalar = vec3::vec3_new(scale_x, scale_y, scale_z);
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
        vec3 translation = vec3::vec3_new(translate_x, translate_y, translate_z);
        for(auto& triangle: mesh->triangles)
        {
            triangle.data.v0 = triangle.data.v0 + translation;
            triangle.data.v1 = triangle.data.v1 + translation;
            triangle.data.v2 = triangle.data.v2 + translation;
        }
    }

    void
    mesh_rotate_x(Mesh* mesh, float angle)
    {
        for(auto& triangle: mesh->triangles)
        {
            auto v0 = triangle.data.v0;
            auto v1 = triangle.data.v1;
            auto v2 = triangle.data.v2;
            // TODO: add support for matrices in mathy
            triangle.data.v0.y = v0.y * cosf(angle) - (v0.z * sinf(angle));
            triangle.data.v0.z = v0.y * sinf(angle) + (v0.z * cosf(angle));

            triangle.data.v1.y = v1.y * cosf(angle) - (v1.z * sinf(angle));
            triangle.data.v1.z = v1.y * sinf(angle) + (v1.z * cosf(angle));

            triangle.data.v2.y = v2.y * cosf(angle) - (v2.z * sinf(angle));
            triangle.data.v2.z = v2.y * sinf(angle) + (v2.z * cosf(angle));
        }
    }

    void
    mesh_rotate_y(Mesh* mesh, float angle)
    {
        for(auto& triangle: mesh->triangles)
        {
            auto v0 = triangle.data.v0;
            auto v1 = triangle.data.v1;
            auto v2 = triangle.data.v2;
            // TODO: add support for matrices in mathy
            triangle.data.v0.x = v0.x * cosf(angle) + v0.z * sinf(angle);
            triangle.data.v0.z = -v0.x * sinf(angle) + v0.z * cosf(angle);

            triangle.data.v1.x = v1.x * cosf(angle) + v1.z * sinf(angle);
            triangle.data.v1.z = -v1.x * sinf(angle) + v1.z * cosf(angle);

            triangle.data.v2.x = v2.x * cosf(angle) + v2.z * sinf(angle);
            triangle.data.v2.z = -v2.x * sinf(angle) + v2.z * cosf(angle);
        }
    }

    void
    mesh_rotate_z(Mesh* mesh, float angle)
    {
        for(auto& triangle: mesh->triangles)
        {
            auto v0 = triangle.data.v0;
            auto v1 = triangle.data.v1;
            auto v2 = triangle.data.v2;
            // TODO: add support for matrices in mathy
            triangle.data.v0.x = v0.x * cosf(angle) - (v0.y * sinf(angle));
            triangle.data.v0.y = v0.x * sinf(angle) + (v0.y * cosf(angle));

            triangle.data.v1.x = v1.x * cosf(angle) - (v1.y * sinf(angle));
            triangle.data.v1.y = v1.x * sinf(angle) + (v1.y * cosf(angle));

            triangle.data.v2.x = v2.x * cosf(angle) - (v2.y * sinf(angle));
            triangle.data.v2.y = v2.x * sinf(angle) + (v2.y * cosf(angle));
        }
    }

    // expects the mesh to be in view space (.i.e camera at the origin)
    void
    mesh_z_division(Mesh* mesh, float fov)
    {
        // TODO: add matrices to mathy
        for(auto& triangle: mesh->triangles)
        {
            float fov_half_angle = fov / 2.0f;
            float scalar = 1.0f / tanf(mathy::to_radians(fov_half_angle));
            triangle.data.v0.x = (triangle.data.v0.x * scalar) / -(triangle.data.v0.z + 0.00001f);
            triangle.data.v0.y = (triangle.data.v0.y * scalar) / -(triangle.data.v0.z + 0.00001f);

            triangle.data.v1.x = (triangle.data.v1.x * scalar) / -(triangle.data.v1.z + 0.00001f);
            triangle.data.v1.y = (triangle.data.v1.y * scalar) / -(triangle.data.v1.z + 0.00001f);

            triangle.data.v2.x = (triangle.data.v2.x * scalar) / -(triangle.data.v2.z + 0.00001f);
            triangle.data.v2.y = (triangle.data.v2.y * scalar) / -(triangle.data.v2.z + 0.00001f);
        }
    }
};
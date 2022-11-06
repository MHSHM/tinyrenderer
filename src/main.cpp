#include "image.h"
#include "triangle.h"
#include "objloader.h"
#include "z-buffer.h"

#include <iostream>
#include <chrono>

static const int width  = 800;
static const int height = 800;

int main()
{
    TGAImage* image = image_new(width, height, TGAImage::RGBA);
    TGAImage* diffuse_texture = image_load("../textures/african_head_diffuse.tga");
    image->flip_vertically(); // i want to have the origin at the left bottom corner of the image
    diffuse_texture->flip_vertically();
    // allocate zbuffer for depth testing
    tiny::Zbuffer* zbuffer = tiny::zbuffer_new(width, height, -1e11f);

    objl::Loader loader;
    loader.LoadFile("../obj/african_head.obj");

    mathy::Vector3<float> light_direction = mathy::Vector3<float>::vec3_new(0.0f, 0.0f, -1.0f);

    auto start = std::chrono::system_clock::now();
    for(int i = 0; i < loader.LoadedIndices.size(); i+=3)
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

        data.v0 = data.v0 * (width / 2.0f) + (width / 2.0f);
        data.v1 = data.v1 * (width / 2.0f) + (width / 2.0f);
        data.v2 = data.v2 * (width / 2.0f) + (width / 2.0f);

        tiny::Triangle triangle = tiny::triangle_new(data);
        tiny::triangle_draw_diffuse(triangle, image, zbuffer, light_direction, diffuse_texture);
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "image was generated at: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
    image->write_tga_file("output.tga");

    while (true);
    
    return 0;
}
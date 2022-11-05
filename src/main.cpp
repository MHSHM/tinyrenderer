#include "image.h"
#include "triangle.h"
#include "objloader.h"
#include "z-buffer.h"

#include <iostream>

static const int width  = 800;
static const int height = 800;

int main()
{
    TGAImage* image = image_new(width, height, TGAImage::RGBA);
    image->flip_vertically(); // i want to have the origin at the left bottom corner of the image
    
    // allocate zbuffer for depth testing
    tiny::Zbuffer* zbuffer = tiny::zbuffer_new(width, height, -1e11f);

    objl::Loader loader;
    loader.LoadFile("../obj/african_head.obj");

    mathy::Vector3<float> light_direction = mathy::Vector3<float>::vec3_new(0.0f, 0.0f, -1.0f);

    for(int i = 0; i < loader.LoadedIndices.size(); i+=3)
    {
        const std::vector<objl::Vertex>& vertices = loader.LoadedVertices;

        mathy::Vector3<float> v0 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i]].Position.X, vertices[loader.LoadedIndices[i]].Position.Y, vertices[loader.LoadedIndices[i]].Position.Z);
        mathy::Vector3<float> v1 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i + 1]].Position.X, vertices[loader.LoadedIndices[i + 1]].Position.Y, vertices[loader.LoadedIndices[i + 1]].Position.Z);
        mathy::Vector3<float> v2 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i + 2]].Position.X, vertices[loader.LoadedIndices[i + 2]].Position.Y, vertices[loader.LoadedIndices[i + 2]].Position.Z);

        mathy::Vector3<float> n0 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i]].Normal.X, vertices[loader.LoadedIndices[i]].Normal.Y, vertices[loader.LoadedIndices[i]].Normal.Z);
        mathy::Vector3<float> n1 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i + 1]].Normal.X, vertices[loader.LoadedIndices[i + 1]].Normal.Y, vertices[loader.LoadedIndices[i + 1]].Normal.Z);
        mathy::Vector3<float> n2 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i + 2]].Normal.X, vertices[loader.LoadedIndices[i + 2]].Normal.Y, vertices[loader.LoadedIndices[i + 2]].Normal.Z);


        v0.x = v0.x * (width / 2.0) + (width / 2.0);
        v1.x = v1.x * (width / 2.0) + (width / 2.0);
        v2.x = v2.x * (width / 2.0) + (width / 2.0);

        v0.y = v0.y * (height / 2.0) + (height / 2.0);
        v1.y = v1.y * (height / 2.0) + (height / 2.0);
        v2.y = v2.y * (height / 2.0) + (height / 2.0);
        
        v0.z = v0.z * (height / 2.0) + (height / 2.0);
        v1.z = v1.z * (height / 2.0) + (height / 2.0);
        v2.z = v2.z * (height / 2.0) + (height / 2.0);

        tiny::Triangle triangle = tiny::triangle_new(v0, v1, v2, n0, n1, n2);
        tiny::triangle_draw_shading(triangle, image, zbuffer, light_direction, white);
    }

    image->write_tga_file("output.tga");

    return 0;
}
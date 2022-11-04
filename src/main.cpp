#include "tgaimage.h"
#include "triangle.h"
#include "objloader.h"

static const TGAColor white   = TGAColor(255, 255, 255, 255);
static const TGAColor red     = TGAColor(255, 0,   0,   255);
static const TGAColor green   = TGAColor(0, 255,   0,   255);
static const TGAColor blue    = TGAColor(0 , 0,   255,   255);

static const int width  = 800;
static const int height = 800;

int main()
{
    TGAImage image(width, height, TGAImage::RGB);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    
    objl::Loader loader;
    loader.LoadFile("../obj/african_head.obj");

    mathy::Vector3<float> light_direction = mathy::Vector3<float>::vec3_new(0.0f, 0.0f, -1.0f);

    for(int i = 0; i < loader.LoadedIndices.size(); i+=3)
    {
        const std::vector<objl::Vertex>& vertices = loader.LoadedVertices;

        mathy::Vector3<float> v0 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i]].Position.X, vertices[loader.LoadedIndices[i]].Position.Y, vertices[loader.LoadedIndices[i]].Position.Z);
        mathy::Vector3<float> v1 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i + 1]].Position.X, vertices[loader.LoadedIndices[i + 1]].Position.Y, vertices[loader.LoadedIndices[i + 1]].Position.Z);
        mathy::Vector3<float> v2 = mathy::Vector3<float>::vec3_new(vertices[loader.LoadedIndices[i + 2]].Position.X, vertices[loader.LoadedIndices[i + 2]].Position.Y, vertices[loader.LoadedIndices[i + 2]].Position.Z);

        v0.x = v0.x * (width / 2.0) + (width / 2.0);
        v1.x = v1.x * (width / 2.0) + (width / 2.0);
        v2.x = v2.x * (width / 2.0) + (width / 2.0);

        v0.y = v0.y * (height / 2.0) + (height / 2.0);
        v1.y = v1.y * (height / 2.0) + (height / 2.0);
        v2.y = v2.y * (height / 2.0) + (height / 2.0);
        
        v0.z = v0.z * (height / 2.0) + (height / 2.0);
        v1.z = v1.z * (height / 2.0) + (height / 2.0);
        v2.z = v2.z * (height / 2.0) + (height / 2.0);

        tiny::Triangle triangle = tiny::triangle_new(v0, v1, v2);
        tiny::triangle_draw_flat_shading(triangle, image, light_direction, white);
    }

    image.write_tga_file("output.tga");
    return 0;
}
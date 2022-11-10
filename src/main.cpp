#include "image.h"
#include "triangle.h"
#include "z-buffer.h"
#include "mesh.h"

#include <iostream>
#include <chrono>

static const TGAColor white   = TGAColor(255, 255, 255, 255);
static const TGAColor red     = TGAColor(255, 0,   0,   255);
static const TGAColor green   = TGAColor(0, 255,   0,   255);
static const TGAColor blue    = TGAColor(0 , 0,   255,   255);

static const int width  = 800;
static const int height = 800;

int main()
{
    TGAImage* image = tiny::image_new(width, height, TGAImage::RGBA);
    TGAImage* diffuse_texture = tiny::image_load("../textures/african_head_diffuse.tga");
    image->flip_vertically(); // i want to have the origin at the left bottom corner of the image
    diffuse_texture->flip_vertically();
    // allocate zbuffer for depth testing
    tiny::Zbuffer* zbuffer = tiny::zbuffer_new(width, height, -1e11f);
    // allocate resource manager
    Resource_Manager* resource_manager = resource_manager_new();
    mathy::Vector3<float> light_direction = mathy::Vector3<float>::vec3_new(0.0f, 0.0f, -1.0f);

    Mesh* mesh = mesh_new();
    mesh->vtable->load(mesh, resource_manager, "../obj/african_head.obj");
    mesh_scale(mesh, (width / 2.0f), (width / 2.0f), (width / 2.0f));
    mesh_translate(mesh, (width / 2.0f), (width / 2.0f), (width / 2.0f));
    
    for(auto& triangle: mesh->triangles)
    {
        tiny::triangle_draw_diffuse(triangle, image, zbuffer, light_direction, diffuse_texture);
    }

    image->write_tga_file("output.tga");

    resource_manager_free(resource_manager);
    while (true);
    
    return 0;
}
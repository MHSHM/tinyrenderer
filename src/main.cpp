#include "image.h"
#include "triangle.h"
#include "z-buffer.h"
#include "mesh.h"
#include "renderer.h"

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
    // allocate resource manager
    Resource_Manager* resource_manager = resource_manager_new();

    // screen buffer
    tiny::Image* image = tiny::image_new(width, height, TGAImage::RGBA);
    image->data->flip_vertically(); // i want to have the origin at the left bottom corner of the image
    resource_manager_add_resource(resource_manager, image, "screen buffer");

    // allocate zbuffer for depth testing
    tiny::Zbuffer* zbuffer = tiny::zbuffer_new(width, height, -1e11f);

    // diffuse texture
    tiny::Image* diffuse_texture = tiny::image_new();
    diffuse_texture->vtable->load(diffuse_texture, resource_manager, "../textures/african_head_diffuse.tga");
    diffuse_texture->data->flip_vertically();
    
    // light
    mathy::Vector3<float> light_direction = mathy::Vector3<float>::vec3_new(0.0f, 0.0f, -1.0f);

    // create a mesh
    tiny::Mesh* mesh = tiny::mesh_new();
    mesh->vtable->load(mesh, resource_manager, "../obj/african_head.obj");
    mesh_scale(mesh, (width / 2.0f), (width / 2.0f), (width / 2.0f));
    mesh_translate(mesh, (width / 2.0f), (width / 2.0f), (width / 2.0f));
    
    // render the mesh
    tiny::render_wireframe(mesh, image, white);

    // write the frame to the disk
    image->data->write_tga_file("output.tga");

    // free all allocated resources
    resource_manager_free(resource_manager);

    while (true);
    
    return 0;
}
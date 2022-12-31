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

using namespace mathy;

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
    resource_manager_add_resource(resource_manager, zbuffer, "z-buffer");

    // diffuse texture
    tiny::Image* diffuse_texture = tiny::image_new();
    diffuse_texture->load(diffuse_texture, resource_manager, "../textures/african_head_diffuse.tga");
    diffuse_texture->data->flip_vertically();

    // light
    vec3 light_direction = vec3::vec3_new(0.0f, 0.0f, -1.0f);

    // camera
    vec3 cam_pos = vec3::vec3_new(0.0f, 0.0f, 0.0f);

    // create a mesh
    tiny::Mesh* mesh = tiny::mesh_new();
    mesh->load(mesh, resource_manager, "../obj/teapot.obj");

    // model space
    mesh_rotate_x(mesh, -mathy::PI * 0.5f);
    mesh_scale(mesh, 0.06f, 0.06f, 0.06f);
    mesh_translate(mesh, 0.0f, -0.5f, -1.5f);

    // view space
    mesh_translate(mesh, -cam_pos.x, -cam_pos.y, -cam_pos.z);

    // perspective projection
    mesh_z_division(mesh, 90.0f);

    // raster space
    mesh_scale(mesh, width / 2.0f, width / 2.0f, 1.0f);
    mesh_translate(mesh, width / 2.0f, width / 2.0f, 0.0f);
    tiny::render_per_pixel_shading(mesh, image, zbuffer, light_direction, white);

    // write the frame to the disk
    image->data->write_tga_file("output.tga");

    // free all allocated resources
    resource_manager_free(resource_manager);

    return 0;
}
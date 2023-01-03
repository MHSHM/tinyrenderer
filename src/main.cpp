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

    // light
    glm::vec3 light_direction = glm::vec3(0.0f, 0.0f, -1.0f);

    // camera
    glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 0.0f);

    // create a mesh
    tiny::Mesh* mesh = tiny::mesh_new();
    mesh->load(mesh, resource_manager, "obj/african_head.obj");

    tiny::render_per_pixel_shading(mesh, image, zbuffer, light_direction, cam_pos, white);

    // write the frame to the disk
    image->data->write_tga_file("output.tga");

    // free all allocated resources
    resource_manager_free(resource_manager);

    return 0;
}
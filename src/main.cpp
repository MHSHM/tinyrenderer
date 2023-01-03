#include "image.h"
#include "triangle.h"
#include "z-buffer.h"
#include "mesh.h"
#include "renderer.h"
#include "shader.h"

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
    tiny::Image* image = tiny::image_new(resource_manager, "screen buffer", width, height, TGAImage::RGBA);
    image->data->flip_vertically(); // i want to have the origin at the left bottom corner of the image

    // allocate zbuffer for depth testing
    tiny::Zbuffer* zbuffer = tiny::zbuffer_new(resource_manager, "z-buffer", width, height, -1e11f);

    tiny::Image* diffuse_texture = tiny::image_new(resource_manager, "diffuse texture");
    diffuse_texture->load(diffuse_texture, resource_manager, "textures/african_head_diffuse.tga");
    diffuse_texture->data->flip_vertically();

    // light
    glm::vec3 light_direction = glm::vec3(0.0f, 0.0f, -1.0f);

    // camera
    glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 1.0f);

    // create a mesh
    tiny::Mesh* mesh = tiny::mesh_new(resource_manager, "african_head");
    mesh->load(mesh, resource_manager, "obj/african_head.obj");

    // create shader
    tiny::Shader* shader_lit = tiny::shader_lit_new(resource_manager, "shader lit");
    tiny::Shader* shader_diffuse = tiny::shader_diffuse_new(resource_manager, "shader diffuse");
    tiny::Shader* shader_wireframe = tiny::shader_wireframe_new(resource_manager, "shader wireframe");

    // set shaders uniforms
    // TODO: find a better way to set uniforms
    tiny::Uniform cam_uniform;
    cam_uniform.kind = tiny::Uniform::VEC3;
    cam_uniform.value.as_vec3 = cam_pos;
    tiny::Uniform light_uniform;
    light_uniform.kind = tiny::Uniform::VEC3;
    light_uniform.value.as_vec3 = light_direction;
    tiny::Uniform diffuse_texture_uniform;
    diffuse_texture_uniform.kind = tiny::Uniform::Texture2D;
    diffuse_texture_uniform.value.as_image = diffuse_texture;

    tiny::shader_set_uniform(shader_lit, "cam_pos", cam_uniform);
    tiny::shader_set_uniform(shader_lit, "light_direction", light_uniform);

    tiny::shader_set_uniform(shader_diffuse, "cam_pos", cam_uniform);
    tiny::shader_set_uniform(shader_diffuse, "light_direction", light_uniform);
    tiny::shader_set_uniform(shader_diffuse, "diffuse_texture", diffuse_texture_uniform);

    tiny::shader_set_uniform(shader_wireframe, "cam_pos", cam_uniform);

    tiny::render_wireframe(mesh, image, shader_wireframe, white);

    // write the frame to the disk
    image->data->write_tga_file("output.tga");

    // free all allocated resources
    resource_manager_free(resource_manager);

    return 0;
}
#pragma once

struct TGAColor;

namespace mathy
{
    template<typename T>
    struct Vector3;
};

namespace tiny 
{
    struct Mesh;
    struct Image;
    struct Line;
    struct Zbuffer;

    void 
    render_wireframe(Mesh* mesh, Image* image, struct Shader* shader, const TGAColor& color);

    void
    render_per_pixel_shading(Mesh* mesh, Image* image, Zbuffer* zbuffer, struct Shader* shader, const TGAColor& color);

    void
    render_with_diffuse(Mesh* mesh, Image* image, tiny::Zbuffer* zbuffer, struct Shader* shader);
};
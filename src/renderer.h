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
    render_point(const glm::vec3& point, Image* image, const TGAColor& color);

    void 
    render_line(const Line& line, Image* image, const TGAColor& color);

    void
    render_mesh_as_points(const Mesh* mesh, Image* image, const TGAColor& color);

    void 
    render_wireframe(const Mesh* mesh, Image* image, const TGAColor& color);

    void
    render_per_pixel_shading(Mesh* mesh, Image* image, Zbuffer* zbuffer, struct Shader* shader, const TGAColor& color);

    void
    render_with_diffuse(Mesh* mesh, Image* image, tiny::Zbuffer* zbuffer, Shader* shader);
};
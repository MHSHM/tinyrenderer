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
    render_fill(const Mesh* mesh, Image* image, const TGAColor& color);

    void 
    render_per_traingle_shading(const Mesh* mesh, Image* image, Zbuffer* zbuffer, const glm::vec3& light_direction, const TGAColor& color);

    void
    render_per_pixel_shading(Mesh* mesh, Image* image, Zbuffer* zbuffer, const glm::vec3& light_direction, const glm::vec3& cam_pos, const TGAColor& color);

    void
    render_with_diffuse(const Mesh* mesh, Image* image, tiny::Zbuffer* zbuffer, const glm::vec3& light_direction, Image* diffuse);
};
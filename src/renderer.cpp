#include "mesh.h"
#include "Math.h"
#include "line.h"
#include "image.h"
#include "AABB.h"
#include "z-buffer.h"
#include "sampler.h"

#include <glm/geometric.hpp>

using namespace mathy;

namespace tiny
{
    inline static bool
    check_pixel_coord(int width, int height, int x, int y)
    {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    inline static bool
    _triangle_is_backfacing(const Triangle& triangle, const glm::vec3& light_direction)
    {
        float dot = mathy::dot(triangle.data.face_normal, light_direction);
        return (dot > 0.00001f);
    }

    inline static bool
    _is_triangle_too_close(const Triangle& triangle, float near_plane)
    {
        float avg_z = (triangle.data.v0.z + triangle.data.v1.z + triangle.data.v2.z) / 3.0f;
        return (abs(avg_z) < near_plane);
    }

    inline static TGAColor
    _triangle_flat_shade(const Triangle& triangle, const TGAColor& color, const glm::vec3& light_direcrion)
    {
        /*
            some notes for future me or anyone else intersted in reading my code
            in OBJ files Vertices are stored in a counter-clockwise order by default
                v2

            v0      v1
            meaning that cross(e0, e1) below will give us a normal coming out from the screen towards us.
            also OBJ file format specification declares that, "A right-hand coordinate system is used to specify the coordinate locations."
            which means that our Z direction is coming out from the screen towards us.
        */

        float dot = mathy::dot(triangle.data.face_normal, -light_direcrion);
        TGAColor shaded_color = TGAColor(color.bgra[2] * dot, color.bgra[1] * dot, color.bgra[0] * dot);

        return shaded_color;
    }

    inline static TGAColor
    _triangle_per_pixel_shading(const glm::vec3& normal, const glm::vec3& light_direction, const TGAColor& color)
    {
        float dot = std::max(mathy::dot(normal, -light_direction), 0.0f);
        TGAColor shaded_color = TGAColor(color.bgra[2] * dot, color.bgra[1] * dot, color.bgra[0] * dot);
        return shaded_color;
    }

    void
    render_point(const glm::vec3& point, Image* image, const TGAColor& color)
    {
        image->data->set((int)point.x, (int)point.y, color);
    }

    void
    render_mesh_as_points(const Mesh* mesh, Image* image, const TGAColor& color)
    {
        for(auto& triangle: mesh->triangles)
        {
            render_point(triangle.data.v0, image, color);
            render_point(triangle.data.v1, image, color);
            render_point(triangle.data.v2, image, color);
        }
    }

    void
    render_line(const Line& line, Image* image, const TGAColor& color)
    {
        int dx = std::abs(line.x1 - line.x0);
        int dy = std::abs(line.y1 - line.y0);
        float step = 1.0f / std::pow(std::max(dx, dy), 2) + 0.0001f;
        for(float t = 0.0f; t <= 1.0f; t += step)
        {
            int x = (1.0f - t) * line.x0 + t * line.x1;
            int y = (1.0f - t) * line.y0 + t * line.y1;
            image->data->set(x, y, color);
        }
    }

    void
    render_wireframe(const Mesh* mesh, Image* image, const TGAColor& color)
    {
        for(auto& triangle: mesh->triangles)
        {
            tiny::Line line0 = tiny::line_new((int)triangle.data.v0.x, (int)triangle.data.v0.y, (int)triangle.data.v1.x, (int)triangle.data.v1.y);
            tiny::Line line1 = tiny::line_new((int)triangle.data.v0.x, (int)triangle.data.v0.y, (int)triangle.data.v2.x, (int)triangle.data.v2.y);
            tiny::Line line2 = tiny::line_new((int)triangle.data.v1.x, (int)triangle.data.v1.y, (int)triangle.data.v2.x, (int)triangle.data.v2.y);

            render_line(line0, image, color);
            render_line(line1, image, color);
            render_line(line2, image, color);
        }
    }

    void
    render_fill(const Mesh* mesh, Image* image, const TGAColor& color)
    {
        for(auto& triangle: mesh->triangles)
        {
            tiny::AABB aabb = tiny::aabb_new(triangle);

            for(int i = aabb.min_x; i <= aabb.max_x; ++i)
            {
                for(int j = aabb.min_y; j <= aabb.max_y; ++j)
                {
                    if(!check_pixel_coord(image->data->width(), image->data->height(), i, j))
                    {
                        continue;
                    }

                    glm::vec2 pixel = glm::vec2((float)i + 0.5f, (float)j + 0.5f);
                    if(auto coord = mathy::is_inside_triangle(pixel, triangle.data.v0, triangle.data.v1, triangle.data.v2); coord.is_inside)
                    {
                        image->data->set(i, j, color);
                    }
                }
            }
        }
    }

    void
    render_per_traingle_shading(const Mesh* mesh, Image* image, Zbuffer* zbuffer, const glm::vec3& light_direction, const TGAColor& color)
    {
        for(auto& triangle: mesh->triangles)
        {
            TGAColor shaded_color = _triangle_flat_shade(triangle, color, light_direction);

            AABB aabb = aabb_new(triangle);

            for(int i = aabb.min_x; i <= aabb.max_x; ++i)
            {
                for(int j = aabb.min_y; j <= aabb.max_y; ++j)
                {
                    if(!check_pixel_coord(image->data->width(), image->data->height(), i, j))
                    {
                        continue;
                    }

                    glm::vec2 pixel = glm::vec2((float)i + 0.5f, (float)j + 0.5f);
                    if(auto coord = mathy::is_inside_triangle(pixel, triangle.data.v0, triangle.data.v1, triangle.data.v2); coord.is_inside)
                    {
                        glm::vec3 pixel_pos = triangle.data.v0 * coord.u + triangle.data.v1 * coord.v + triangle.data.v2 * coord.w;
                        if(pixel_pos.z > zbuffer->depths[i * zbuffer->height + j])
                        {
                            image->data->set(i, j, shaded_color);
                            zbuffer->depths[i * zbuffer->height + j] = pixel_pos.z;
                        }
                    }
                }
            }
        }
    }

    void
    render_per_pixel_shading(Mesh* mesh, Image* image, Zbuffer* zbuffer, const glm::vec3& light_direction, const glm::vec3& cam_pos, const TGAColor& color)
    {
        for(auto& triangle: mesh->triangles)
        {
            // perform different transformations
            // model matrix
            glm::mat4 rotate_y{ 1.0f }, scale{ 1.0f }, translate{ 1.0f };
            mathy::scale_mat(scale, glm::vec3(1.87f, 1.87f, 1.87f));
            mathy::translation_mat(translate, glm::vec3(0.0f, -0.5f, -3.5f));
            glm::mat4 model_mat = translate * scale * rotate_y;

            // view matrix
            glm::mat4 view{ 1.0f };
            mathy::view_mat(view, cam_pos);

            // proj matrix
            glm::mat4 proj{ 1.0f };
            mathy::perspective_projection_mat(proj, 0.1f, 100.0f, 90.0f);

            // model-view-proj transformation
            glm::vec4 homogenous_v0 = proj * view * model_mat * glm::vec4(triangle.data.v0, 1.0f);
            glm::vec4 homogenous_v1 = proj * view * model_mat * glm::vec4(triangle.data.v1, 1.0f);
            glm::vec4 homogenous_v2 = proj * view * model_mat * glm::vec4(triangle.data.v2, 1.0f);

            // homogenous to cartesian
            // TODO: fix this nonsense
            float z0 = triangle.data.v0.z;
            float z1 = triangle.data.v1.z;
            float z2 = triangle.data.v2.z;
            triangle.data.v0 = glm::vec3(homogenous_v0) / homogenous_v0.w;
            triangle.data.v1 = glm::vec3(homogenous_v1) / homogenous_v1.w;
            triangle.data.v2 = glm::vec3(homogenous_v2) / homogenous_v2.w;
            triangle.data.v0.z = z0;
            triangle.data.v1.z = z1;
            triangle.data.v2.z = z2;

            // TODO: this wrong will fix when implementing shaders
            if (triangle.data.v0.x > 1.0f || triangle.data.v0.x < -1.0f || triangle.data.v0.y > 1.0f || triangle.data.v0.y < -1.0f) continue;
            if (triangle.data.v1.x > 1.0f || triangle.data.v1.x < -1.0f || triangle.data.v1.y > 1.0f || triangle.data.v1.y < -1.0f) continue;
            if (triangle.data.v2.x > 1.0f || triangle.data.v2.x < -1.0f || triangle.data.v2.y > 1.0f || triangle.data.v2.y < -1.0f) continue;

            // viewport matrix
            int viewport_w = image->data->width();
            int viewport_h = image->data->height();
            glm::mat4 viewport_scale{ 1.0f }, viewport_translate{ 1.0f };
            mathy::scale_mat(viewport_scale, glm::vec3(viewport_w / 2.0f, viewport_h / 2.0f, 1.0f));
            mathy::translation_mat(viewport_translate, glm::vec3(viewport_w / 2.0f, viewport_h / 2.0f, 0.0f));

            // viewport tranformation
            triangle.data.v0 = glm::vec3(viewport_translate * viewport_scale * glm::vec4(triangle.data.v0, 1.0f));
            triangle.data.v1 = glm::vec3(viewport_translate * viewport_scale * glm::vec4(triangle.data.v1, 1.0f));
            triangle.data.v2 = glm::vec3(viewport_translate * viewport_scale * glm::vec4(triangle.data.v2, 1.0f));

            // rasterization
            AABB aabb = aabb_new(triangle);

            for(int i = aabb.min_x; i <= aabb.max_x; ++i)
            {
                for(int j = aabb.min_y; j <= aabb.max_y; ++j)
                {
                    if(!check_pixel_coord(image->data->width(), image->data->height(), i, j))
                    {
                        continue;
                    }

                    glm::vec2 pixel = glm::vec2((float)i + 0.5f, (float)j + 0.5f);
                    if(auto coord = mathy::is_inside_triangle(pixel, triangle.data.v0, triangle.data.v1, triangle.data.v2); coord.is_inside)
                    {
                        glm::vec3 pixel_normal = triangle.data.n0 * coord.u + triangle.data.n1 * coord.v + triangle.data.n2 * coord.w;
                        glm::vec3 pixel_pos    = triangle.data.v0 * coord.u + triangle.data.v1 * coord.v + triangle.data.v2 * coord.w;
                        glm::vec2 pixel_uv     = triangle.data.uv0 * coord.u + triangle.data.uv1 * coord.v + triangle.data.uv2 * coord.w;
                        // depth test
                        if(pixel_pos.z > zbuffer->depths[i * zbuffer->height + j])
                        {
                            TGAColor shaded_color = _triangle_per_pixel_shading(glm::normalize(pixel_normal), light_direction, color);
                            image->data->set(i, j, shaded_color);
                            zbuffer->depths[i * zbuffer->height + j] = pixel_pos.z;
                        }
                    }
                }
            }
        }
    }

    void
    render_with_diffuse(const Mesh* mesh, Image* image, tiny::Zbuffer* zbuffer, const glm::vec3& light_direction, Image* diffuse)
    {
        for(auto& triangle: mesh->triangles)
        {
            AABB aabb = aabb_new(triangle);

            for(int i = aabb.min_x; i <= aabb.max_x; ++i)
            {
                for(int j = aabb.min_y; j <= aabb.max_y; ++j)
                {
                    if(!check_pixel_coord(image->data->width(), image->data->height(), i, j))
                    {
                        continue;
                    }

                    glm::vec2 pixel = glm::vec2((float)i + 0.5f, (float)j + 0.5f);
                    if(auto coord = mathy::is_inside_triangle(pixel, triangle.data.v0, triangle.data.v1, triangle.data.v2); coord.is_inside)
                    {
                        glm::vec3 pixel_normal = triangle.data.n0 * coord.u + triangle.data.n1 * coord.v + triangle.data.n2 * coord.w;
                        glm::vec3 pixel_pos    = triangle.data.v0 * coord.u + triangle.data.v1 * coord.v + triangle.data.v2 * coord.w;
                        glm::vec2 pixel_uv     = triangle.data.uv0 * coord.u + triangle.data.uv1 * coord.v + triangle.data.uv2 * coord.w;

                        // depth test
                        if(pixel_pos.z > zbuffer->depths[i * zbuffer->height + j])
                        {
                            TGAColor shaded_color = _triangle_per_pixel_shading(glm::normalize(pixel_normal),
                                                    light_direction,
                                                    sample(diffuse, pixel_uv.x, pixel_uv.y));
                            image->data->set(i, j, shaded_color);
                            zbuffer->depths[i * zbuffer->height + j] = pixel_pos.z;
                        }
                    }
                }
            }
        }
    }
};
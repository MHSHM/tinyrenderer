#include "mesh.h"
#include "Math.h"
#include "line.h"
#include "image.h"
#include "AABB.h"
#include "z-buffer.h"
#include "sampler.h"
#include "shader.h"

#include <glm/geometric.hpp>

using namespace mathy;

namespace tiny
{
    inline static bool
    _triangle_is_backfacing(const Triangle& triangle, const glm::vec3& light_direction)
    {
        float dot = mathy::dot(triangle.data.face_normal, light_direction);
        return (dot > 0.00001f);
    }

    inline static void
    _render_line(const Line& line, Image* image, const TGAColor& color)
    {
        int dx = std::abs(line.x1 - line.x0);
        int dy = std::abs(line.y1 - line.y0);
        float step = 1.0f / std::pow(std::max(dx, dy), 2) + 0.0001f;
        for (float t = 0.0f; t <= 1.0f; t += step)
        {
            int x = (1.0f - t) * line.x0 + t * line.x1;
            int y = (1.0f - t) * line.y0 + t * line.y1;
            image->data->set(x, y, color);
        }
    }

    void
    render_wireframe(Mesh* mesh, Image* image, Shader* shader, const TGAColor& color)
    {
        for (auto& triangle : mesh->triangles)
        {
            // run vertex shader on all triangles
            shader->vertex_shader(shader, triangle);

            // if a single vertex of the triangle's 3 vertices is outside of the
            // projection plane CLIP the whole triangle. Why doing this? well..
            // too lazy to implement a sophisticated clipping algorithm
            if (triangle.data.v0.x > 1.0f || triangle.data.v0.x < -1.0f || triangle.data.v0.y > 1.0f || triangle.data.v0.y < -1.0f) continue;
            if (triangle.data.v1.x > 1.0f || triangle.data.v1.x < -1.0f || triangle.data.v1.y > 1.0f || triangle.data.v1.y < -1.0f) continue;
            if (triangle.data.v2.x > 1.0f || triangle.data.v2.x < -1.0f || triangle.data.v2.y > 1.0f || triangle.data.v2.y < -1.0f) continue;

            // convert to viewport space
            int viewport_w = image->data->width();
            int viewport_h = image->data->height();
            glm::mat4 viewport_scale{ 1.0f }, viewport_translate{ 1.0f };
            mathy::scale_mat(viewport_scale, glm::vec3(viewport_w / 2.0f, viewport_h / 2.0f, 1.0f));
            mathy::translation_mat(viewport_translate, glm::vec3(viewport_w / 2.0f, viewport_h / 2.0f, 0.0f));

            // viewport tranformation
            triangle.data.v0 = glm::vec3(viewport_translate * viewport_scale * glm::vec4(triangle.data.v0, 1.0f));
            triangle.data.v1 = glm::vec3(viewport_translate * viewport_scale * glm::vec4(triangle.data.v1, 1.0f));
            triangle.data.v2 = glm::vec3(viewport_translate * viewport_scale * glm::vec4(triangle.data.v2, 1.0f));

            tiny::Line line0 = tiny::line_new((int)triangle.data.v0.x, (int)triangle.data.v0.y, (int)triangle.data.v1.x, (int)triangle.data.v1.y);
            tiny::Line line1 = tiny::line_new((int)triangle.data.v0.x, (int)triangle.data.v0.y, (int)triangle.data.v2.x, (int)triangle.data.v2.y);
            tiny::Line line2 = tiny::line_new((int)triangle.data.v1.x, (int)triangle.data.v1.y, (int)triangle.data.v2.x, (int)triangle.data.v2.y);

            _render_line(line0, image, color);
            _render_line(line1, image, color);
            _render_line(line2, image, color);
        }
    }

    void
    render_per_pixel_shading(Mesh* mesh, Image* image, Zbuffer* zbuffer, Shader* shader, const TGAColor& color)
    {
        for (auto& triangle: mesh->triangles)
        {
            // run vertex shader on all triangles
            shader->vertex_shader(shader, triangle);

            // if a single vertex of the triangle's 3 vertices is outside of the
            // projection plane CLIP the whole triangle. Why doing this? well..
            // too lazy to implement a sophisticated clipping algorithm
            if (triangle.data.v0.x > 1.0f || triangle.data.v0.x < -1.0f || triangle.data.v0.y > 1.0f || triangle.data.v0.y < -1.0f) continue;
            if (triangle.data.v1.x > 1.0f || triangle.data.v1.x < -1.0f || triangle.data.v1.y > 1.0f || triangle.data.v1.y < -1.0f) continue;
            if (triangle.data.v2.x > 1.0f || triangle.data.v2.x < -1.0f || triangle.data.v2.y > 1.0f || triangle.data.v2.y < -1.0f) continue;

            // convert to viewport space
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

            for (int i = aabb.min_x; i <= aabb.max_x; ++i)
            {
                for (int j = aabb.min_y; j <= aabb.max_y; ++j)
                {
                    glm::vec2 pixel = glm::vec2((float)i + 0.5f, (float)j + 0.5f);
                    if (auto coord = mathy::is_inside_triangle(pixel, triangle.data.v0, triangle.data.v1, triangle.data.v2); coord.is_inside)
                    {
                        glm::vec3 pixel_normal = triangle.data.n0 * coord.u + triangle.data.n1 * coord.v + triangle.data.n2 * coord.w;
                        glm::vec3 pixel_pos = triangle.data.v0 * coord.u + triangle.data.v1 * coord.v + triangle.data.v2 * coord.w;
                        glm::vec2 pixel_uv = triangle.data.uv0 * coord.u + triangle.data.uv1 * coord.v + triangle.data.uv2 * coord.w;

                        // depth test
                        if (pixel_pos.z > zbuffer->depths[i * zbuffer->height + j])
                        {
                            TGAColor shaded_color = shader->fragment_shader(shader, pixel_normal, color);
                            image->data->set(i, j, shaded_color);
                            zbuffer->depths[i * zbuffer->height + j] = pixel_pos.z;
                        }
                    }
                }
            }
        }
    }

    void
    render_with_diffuse(Mesh* mesh, Image* image, tiny::Zbuffer* zbuffer, Shader* shader)
    {
        Image* diffuse = shader->uniforms["diffuse_texture"].value.as_image;

        for (auto& triangle : mesh->triangles)
        {
            // run vertex shader on all triangles
            shader->vertex_shader(shader, triangle);

            // if a single vertex of the triangle's 3 vertices is outside of the
            // projection plane CLIP the whole triangle. Why doing this? well..
            // too lazy to implement a sophisticated clipping algorithm
            if (triangle.data.v0.x > 1.0f || triangle.data.v0.x < -1.0f || triangle.data.v0.y > 1.0f || triangle.data.v0.y < -1.0f) continue;
            if (triangle.data.v1.x > 1.0f || triangle.data.v1.x < -1.0f || triangle.data.v1.y > 1.0f || triangle.data.v1.y < -1.0f) continue;
            if (triangle.data.v2.x > 1.0f || triangle.data.v2.x < -1.0f || triangle.data.v2.y > 1.0f || triangle.data.v2.y < -1.0f) continue;

            // convert to viewport space
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

            for (int i = aabb.min_x; i <= aabb.max_x; ++i)
            {
                for (int j = aabb.min_y; j <= aabb.max_y; ++j)
                {
                    glm::vec2 pixel = glm::vec2((float)i + 0.5f, (float)j + 0.5f);
                    if (auto coord = mathy::is_inside_triangle(pixel, triangle.data.v0, triangle.data.v1, triangle.data.v2); coord.is_inside)
                    {
                        glm::vec3 pixel_normal = triangle.data.n0 * coord.u + triangle.data.n1 * coord.v + triangle.data.n2 * coord.w;
                        glm::vec3 pixel_pos = triangle.data.v0 * coord.u + triangle.data.v1 * coord.v + triangle.data.v2 * coord.w;
                        glm::vec2 pixel_uv = triangle.data.uv0 * coord.u + triangle.data.uv1 * coord.v + triangle.data.uv2 * coord.w;

                        // depth test
                        if (pixel_pos.z > zbuffer->depths[i * zbuffer->height + j])
                        {
                            TGAColor shaded_color = shader->fragment_shader(shader, pixel_normal, sample(diffuse, pixel_uv.x, pixel_uv.y));
                            image->data->set(i, j, shaded_color);
                            zbuffer->depths[i * zbuffer->height + j] = pixel_pos.z;
                        }
                    }
                }
            }
        }
    }
};
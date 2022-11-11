#include "triangle.h"
#include "line.h"
#include "AABB.h"
#include "sampler.h"

namespace tiny 
{
    inline static bool
    _triangle_is_backfacing(const Triangle& triangle, const mathy::Vector3<float>& light_direction)
    {
        float dot = mathy::Vector3<float>::dot(triangle.data.face_normal, light_direction);
        return (dot > 0.00001f);
    }
    
    inline static TGAColor
    _triangle_flat_shade(const Triangle& triangle, const TGAColor& color, const mathy::Vector3<float>& light_direcrion)
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

        float dot = mathy::Vector3<float>::dot(triangle.data.face_normal, -light_direcrion);
        TGAColor shaded_color = TGAColor(color.bgra[2] * dot, color.bgra[1] * dot, color.bgra[0] * dot);

        return shaded_color;
    }

    inline static TGAColor
    _triangle_per_pixel_shading(const mathy::Vector3<float>& normal, const mathy::Vector3<float>& light_direction, const TGAColor& color)
    {
        float dot = std::max(mathy::Vector3<float>::dot(normal, -light_direction), 0.0f);
        TGAColor shaded_color = TGAColor(color.bgra[2] * dot, color.bgra[1] * dot, color.bgra[0] * dot);
        return shaded_color;
    }

    Triangle
    triangle_new(const Triangle_Data& data)
    {
        Triangle triangle;
        triangle.data.v0 = data.v0;
        triangle.data.v1 = data.v1;
        triangle.data.v2 = data.v2;
        triangle.data.n0 = data.n0;
        triangle.data.n1 = data.n1;
        triangle.data.n2 = data.n2;
        triangle.data.uv0 = data.uv0;
        triangle.data.uv1 = data.uv1;
        triangle.data.uv2 = data.uv2;

        mathy::Vector3<float> e0 = triangle.data.v1 - triangle.data.v0;
        mathy::Vector3<float> e1 = triangle.data.v2 - triangle.data.v0;
        triangle.data.face_normal = mathy::Vector3<float>::cross(e0, e1);
        triangle.data.face_normal = mathy::Vector3<float>::normalize(triangle.data.face_normal);
        
        return triangle;
    }

    void
    triangle_draw_wireframe(const Triangle& triangle, Image* image, const TGAColor& color)
    {
        tiny::Line line0 = tiny::line_new((int)triangle.data.v0.x, (int)triangle.data.v0.y, (int)triangle.data.v1.x, (int)triangle.data.v1.y);
        tiny::Line line1 = tiny::line_new((int)triangle.data.v0.x, (int)triangle.data.v0.y, (int)triangle.data.v2.x, (int)triangle.data.v2.y);
        tiny::Line line2 = tiny::line_new((int)triangle.data.v1.x, (int)triangle.data.v1.y, (int)triangle.data.v2.x, (int)triangle.data.v2.y);

        tiny::line_draw(line0, image, color);
        tiny::line_draw(line1, image, color);
        tiny::line_draw(line2, image, color);
    }

    void
    triangle_draw_fill(const Triangle& triangle, Image* image, const TGAColor& color)
    {
        AABB aabb = aabb_new(triangle);

        for(int i = aabb.min_x; i <= aabb.max_x; ++i)
        {
            for(int j = aabb.min_y; j <= aabb.max_y; ++j)
            {
                mathy::Vector2 pixel = mathy::Vector2<float>::vec2_new((float)i + 0.5f, (float)j + 0.5f);
                if(auto coord = mathy::is_inside_triangle(pixel, triangle.data.v0, triangle.data.v1, triangle.data.v2); coord.is_inside)
                {
                    image->data->set(i, j, color);
                }
            }
        }
    }

    void
    triangle_draw_flat_shading(const Triangle& triangle, Image* image, tiny::Zbuffer* zbuffer, const mathy::Vector3<float>& light_direction, const TGAColor& color)
    {
        if(_triangle_is_backfacing(triangle, light_direction))
        {
            return;
        }

        TGAColor shaded_color = _triangle_flat_shade(triangle, color, light_direction);

        AABB aabb = aabb_new(triangle);
        
        for(int i = aabb.min_x; i <= aabb.max_x; ++i)
        {
            for(int j = aabb.min_y; j <= aabb.max_y; ++j)
            {
                mathy::Vector2 pixel = mathy::Vector2<float>::vec2_new((float)i + 0.5f, (float)j + 0.5f);
                if(auto coord = mathy::is_inside_triangle(pixel, triangle.data.v0, triangle.data.v1, triangle.data.v2); coord.is_inside)
                {
                    mathy::Vector3<float> pixel_pos = triangle.data.v0 * coord.u + triangle.data.v1 * coord.v + triangle.data.v2 * coord.w;
                    if(pixel_pos.z > zbuffer->depths[i * zbuffer->height + j])
                    {
                        image->data->set(i, j, shaded_color);
                        zbuffer->depths[i * zbuffer->height + j] = pixel_pos.z;
                    }
                }
            }
        }
    }

    void
    triangle_draw_shading(const Triangle& triangle, Image* image, tiny::Zbuffer* zbuffer, const mathy::Vector3<float>& light_direction, const TGAColor& color)
    {
        if(_triangle_is_backfacing(triangle, light_direction))
        {
            return;
        }

        AABB aabb = aabb_new(triangle);
        
        for(int i = aabb.min_x; i <= aabb.max_x; ++i)
        {
            for(int j = aabb.min_y; j <= aabb.max_y; ++j)
            {
                mathy::Vector2 pixel = mathy::Vector2<float>::vec2_new((float)i + 0.5f, (float)j + 0.5f);
                if(auto coord = mathy::is_inside_triangle(pixel, triangle.data.v0, triangle.data.v1, triangle.data.v2); coord.is_inside)
                {
                    mathy::Vector3<float> pixel_normal = triangle.data.n0 * coord.u + triangle.data.n1 * coord.v + triangle.data.n2 * coord.w;
                    mathy::Vector3<float> pixel_pos    = triangle.data.v0 * coord.u + triangle.data.v1 * coord.v + triangle.data.v2 * coord.w;
                    mathy::Vector2<float> pixel_uv     = triangle.data.uv0 * coord.u + triangle.data.uv1 * coord.v + triangle.data.uv2 * coord.w;
                    // depth test
                    if(pixel_pos.z > zbuffer->depths[i * zbuffer->height + j])
                    {
                        TGAColor shaded_color = _triangle_per_pixel_shading(mathy::Vector3<float>::normalize(pixel_normal), light_direction, color);
                        image->data->set(i, j, shaded_color);
                        zbuffer->depths[i * zbuffer->height + j] = pixel_pos.z;
                    }
                }
            }
        }
    }

    void
    triangle_draw_diffuse(const Triangle& triangle, Image* image, tiny::Zbuffer* zbuffer, const mathy::Vector3<float>& light_direction, Image* diffuse)
    {
        if(_triangle_is_backfacing(triangle, light_direction))
        {
            return;
        }

        AABB aabb = aabb_new(triangle);
        
        for(int i = aabb.min_x; i <= aabb.max_x; ++i)
        {
            for(int j = aabb.min_y; j <= aabb.max_y; ++j)
            {
                mathy::Vector2 pixel = mathy::Vector2<float>::vec2_new((float)i + 0.5f, (float)j + 0.5f);
                if(auto coord = mathy::is_inside_triangle(pixel, triangle.data.v0, triangle.data.v1, triangle.data.v2); coord.is_inside)
                {
                    mathy::Vector3<float> pixel_normal = triangle.data.n0 * coord.u + triangle.data.n1 * coord.v + triangle.data.n2 * coord.w;
                    mathy::Vector3<float> pixel_pos    = triangle.data.v0 * coord.u + triangle.data.v1 * coord.v + triangle.data.v2 * coord.w;
                    mathy::Vector2<float> pixel_uv     = triangle.data.uv0 * coord.u + triangle.data.uv1 * coord.v + triangle.data.uv2 * coord.w;
                    
                    // depth test
                    if(pixel_pos.z > zbuffer->depths[i * zbuffer->height + j])
                    {
                        TGAColor shaded_color = _triangle_per_pixel_shading(mathy::Vector3<float>::normalize(pixel_normal),
                                                light_direction,
                                                sample(diffuse, pixel_uv.x, pixel_uv.y));
                        image->data->set(i, j, shaded_color);
                        zbuffer->depths[i * zbuffer->height + j] = pixel_pos.z;
                    }
                }
            }
        }
    }
};
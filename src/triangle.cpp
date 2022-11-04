#include "triangle.h"
#include "line.h"
#include "AABB.h"

namespace tiny 
{
    inline static bool
    _triangle_is_backfacing(const Triangle& triangle, const mathy::Vector3<float>& light_direction)
    {
        float dot = mathy::Vector3<float>::dot(triangle.face_normal, -light_direction);
        return (dot <= 0.0f);
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

        float dot = mathy::Vector3<float>::dot(triangle.face_normal, -light_direcrion);
        TGAColor shaded_color = TGAColor(color.bgra[2] * dot, color.bgra[1] * dot, color.bgra[0] * dot);

        return shaded_color;
    }

    Triangle
    triangle_new(const mathy::Vector3<float>& v0, const mathy::Vector3<float>& v1, const mathy::Vector3<float>& v2)
    {
        Triangle triangle;
        triangle.v0 = v0;
        triangle.v1 = v1;
        triangle.v2 = v2;
        
        mathy::Vector3<float> e0 = triangle.v1 - triangle.v0;
        mathy::Vector3<float> e1 = triangle.v2 - triangle.v0;
        triangle.face_normal = mathy::Vector3<float>::cross(e0, e1);
        triangle.face_normal = mathy::Vector3<float>::normalize(triangle.face_normal);
        
        return triangle;
    }

    void
    triangle_draw_wireframe(const Triangle& triangle, TGAImage& image, const TGAColor& color)
    {
        tiny::Line line0 = tiny::line_new((int)triangle.v0.x, (int)triangle.v0.y, (int)triangle.v1.x, (int)triangle.v1.y);
        tiny::Line line1 = tiny::line_new((int)triangle.v0.x, (int)triangle.v0.y, (int)triangle.v2.x, (int)triangle.v2.y);
        tiny::Line line2 = tiny::line_new((int)triangle.v1.x, (int)triangle.v1.y, (int)triangle.v2.x, (int)triangle.v2.y);

        tiny::line_draw(line0, image, color);
        tiny::line_draw(line1, image, color);
        tiny::line_draw(line2, image, color);
    }

    void
    triangle_draw_fill(const Triangle& triangle, TGAImage& image, const TGAColor& color)
    {
        AABB aabb = aabb_new(triangle);

        for(int i = aabb.min_x; i <= aabb.max_x; ++i)
        {
            for(int j = aabb.min_y; j <= aabb.max_y; ++j)
            {
                mathy::Vector2 pixel = mathy::Vector2<float>::vec2_new((float)i + 0.5f, (float)j + 0.5f);
                if(auto coord = mathy::is_inside_triangle(pixel, triangle.v0, triangle.v1, triangle.v2); coord.is_inside)
                {
                    image.set(i, j, color);
                }
            }
        }
    }

    void
    triangle_draw_flat_shading(const Triangle& triangle, TGAImage& image, const mathy::Vector3<float>& light_direction, const TGAColor& color)
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
                if(auto coord = mathy::is_inside_triangle(pixel, triangle.v0, triangle.v1, triangle.v2); coord.is_inside)
                {
                    image.set(i, j, shaded_color);
                }
            }
        }
    }

};
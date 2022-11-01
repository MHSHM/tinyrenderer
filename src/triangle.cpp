#include "triangle.h"
#include "line.h"
#include "AABB.h"

namespace tiny 
{
    Triangle
    triangle_new(const mathy::Vector3<float>& v0, const mathy::Vector3<float>& v1, const mathy::Vector3<float>& v2)
    {
        Triangle triangle;
        triangle.v0 = v0;
        triangle.v1 = v1;
        triangle.v2 = v2;
        return triangle;
    }

    void
    triangle_draw_wireframe(const Triangle& triangle, TGAImage& image, TGAColor color)
    {
        tiny::Line line0 = tiny::line_new((int)triangle.v0.x, (int)triangle.v0.y, (int)triangle.v1.x, (int)triangle.v1.y);
        tiny::Line line1 = tiny::line_new((int)triangle.v0.x, (int)triangle.v0.y, (int)triangle.v2.x, (int)triangle.v2.y);
        tiny::Line line2 = tiny::line_new((int)triangle.v1.x, (int)triangle.v1.y, (int)triangle.v2.x, (int)triangle.v2.y);

        tiny::line_draw(line0, image, color);
        tiny::line_draw(line1, image, color);
        tiny::line_draw(line2, image, color);
    }

    void
    triangle_draw_fill(const Triangle& triangle, TGAImage& image, TGAColor color)
    {
        AABB aabb = aabb_new(triangle);

        for(int i = aabb.min_x; i < aabb.max_x; ++i)
        {
            for(int j = aabb.min_y; j < aabb.max_y; ++j)
            {
                image.set(i, j, color);
            }
        }
    }
};
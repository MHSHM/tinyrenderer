#include "AABB.h"
#include "triangle.h"

namespace tiny 
{
    AABB
    aabb_new(const Triangle& triangle)
    {
        AABB aabb;

        glm::ivec2 pixel_0 = glm::ivec2((int)triangle.data.v0.x, (int)triangle.data.v0.y);
        glm::ivec2 pixel_1 = glm::ivec2((int)triangle.data.v1.x, (int)triangle.data.v1.y);
        glm::ivec2 pixel_2 = glm::ivec2((int)triangle.data.v2.x, (int)triangle.data.v2.y);

        int min_x = std::min(pixel_0.x, (int)1e11);
        min_x = std::min(min_x, pixel_1.x);
        min_x = std::min(min_x, pixel_2.x);
        int max_x = std::max(pixel_0.x, (int)1e-11);
        max_x = std::max(max_x, pixel_1.x);
        max_x = std::max(max_x, pixel_2.x);
        int min_y = std::min(pixel_0.y, (int)1e11);
        min_y = std::min(min_y, pixel_1.y);
        min_y = std::min(min_y, pixel_2.y);
        int max_y = std::max(pixel_0.y, (int)1e-11);
        max_y = std::max(max_y, pixel_1.y);
        max_y = std::max(max_y, pixel_2.y);

        aabb.min_x = min_x;
        aabb.min_y = min_y;
        aabb.max_x = max_x;
        aabb.max_y = max_y;

        return aabb;
    }
};